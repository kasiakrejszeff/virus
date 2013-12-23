#ifndef _VIRUS_GENEALOGY_
#define _VIRUS_GENEALOGY_

#include <exception>
#include <map>
#include <set>
#include <vector>
#include <memory>

// TODO Usunąć w ostatecznej wersji.
#include <iostream>

//
// Wyjątki
//

class VirusAlreadyCreated : public std::exception {
	virtual const char* what() const noexcept {
		return "VirusAlreadyCreated";
	}
};

class VirusNotFound : public std::exception {
	virtual const char* what() const noexcept {
		return "VirusNotFound";
	}
};

class TriedToRemoveStemVirus : public std::exception {
	virtual const char* what() const noexcept {
		return "TriedToRemoveStemVirus";
	}
};

//
// Szablon klasy VirusGenealogy
//

template <class Virus>
class VirusGenealogy {

	class VirusNode;
	typedef std::map<typename Virus::id_type, VirusNode *> virus_graph;
	typedef typename virus_graph::iterator virus_graph_iterator;

	typedef std::shared_ptr<VirusNode> node_shared_ptr;
	typedef std::  weak_ptr<VirusNode> node_weak_ptr;

public:

	VirusGenealogy(typename Virus::id_type const &stem_id)
		: stem_id(stem_id)
	{
		node_shared_ptr node_ptr(new VirusNode(stem_id, &graph));
		std::pair<virus_graph_iterator, bool> result = graph.insert(make_pair(stem_id, node_ptr.get()));
		node_ptr->set_position(result.first);
		stem_node_ptr = node_ptr;
	};

	typename Virus::id_type get_stem_id() const noexcept {
		return stem_id;
	}

	std::vector<typename Virus::id_type> get_children(typename Virus::id_type const &id) const {
		if (!exists(id))
			throw VirusNotFound();

		std::vector<typename Virus::id_type> result;
		for (auto child : graph.find(id)->second->children)
			result.push_back(child->id);

		return result;
	}

	std::vector<typename Virus::id_type> get_parents (typename Virus::id_type const &id) const {
		if (!exists(id))
			throw VirusNotFound();

		std::vector<typename Virus::id_type> result;
		for (auto parent : graph.find(id)->second->parents)
			result.push_back(parent->id);

		return result;
	}

	bool exists(typename Virus::id_type const &id) const {
		return (graph.find(id) != graph.end());
	}

	Virus& operator[](typename Virus::id_type const &id) const {
		if (!exists(id))
			throw VirusNotFound();

		return graph.find(id)->second->my_virus;
	}

	void create(typename Virus::id_type const &id, typename Virus::id_type const &parent_id) {
		std::vector<typename Virus::id_type> v;
		v.push_back(parent_id);
		create(id, v);
	}

	void create(typename Virus::id_type const &id, std::vector<typename Virus::id_type> const &parent_ids) {

		if (graph.find(id) != graph.end())
			throw VirusAlreadyCreated();

		// Dziecko otrzymuje wskaźniki na swoich poprzedników.
		node_shared_ptr node_ptr(new VirusNode(id, &graph));

		for (auto &parent_id : parent_ids) {
			auto tmp = graph.find(parent_id);

			if (tmp == graph.end())
				throw VirusNotFound();

			node_ptr->parents.insert(tmp->second);
		}

		auto parent_ptr_it = node_ptr->parents.begin();

		// Początek modyfikacji zawartości grafu. Jeśli zostanie
		// rzucony wyjątek, modyfikacja zostanie cofnięta.

		std::pair<virus_graph_iterator, bool> result = graph.insert(make_pair(id, node_ptr.get()));
		node_ptr->set_position(result.first);
		assert(result.second); // ?? 

		try {
			for (; parent_ptr_it != node_ptr->parents.end(); ++parent_ptr_it) {
				(*parent_ptr_it)->children.insert(node_shared_ptr(node_ptr));
			}
		}
		catch (...) {
			for (; parent_ptr_it != node_ptr->parents.begin();) {
				--parent_ptr_it;
				(*parent_ptr_it)->children.erase(node_shared_ptr(node_ptr));
			}

			graph.erase(result.first);
			throw; // Rzucamy aktywny wyjątek dalej
		}
	}

	void connect(typename Virus::id_type const &child_id, typename Virus::id_type const &parent_id);
	void remove(typename Virus::id_type const &id) {


	}

private:

	virus_graph graph;
	typename Virus::id_type stem_id;
	node_shared_ptr stem_node_ptr;
	
	// Konstruktor kopiujący oraz operator przypisania mają być niedostępne:
	VirusGenealogy(VirusGenealogy<Virus> const &) = delete;
	VirusGenealogy& operator=(VirusGenealogy<Virus> const &) = delete;

	//
	// Klasa reprezentująca wierzchołek w grafie
	//

	class VirusNode {
	public:
		std::set<node_shared_ptr> children;
		std::set<VirusNode *>     parents;

		virus_graph * containing_graph;
		virus_graph_iterator position;

		typename Virus::id_type id;
		Virus my_virus;

		VirusNode(typename Virus::id_type id, virus_graph * g) :
			containing_graph(g),
			id(id),
			my_virus(Virus(id))
		{ }

		~VirusNode() {
			if (containing_graph != nullptr)
				containing_graph->erase(position);
		}

		void set_position(virus_graph_iterator const &it) {
			position = it;
		}
	};
};

#endif