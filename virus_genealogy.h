#ifndef _VIRUS_GENEALOGY_
#define _VIRUS_GENEALOGY_

#include <exception>
#include <map>
#include <set>
#include <vector>
#include <memory>

//
// Wyjątki
//

class VirusAlreadyCreated : public std::exception {
public:
	virtual const char* what() const noexcept {
		return "VirusAlreadyCreated";
	}
};

class VirusNotFound : public std::exception {
public:
	virtual const char* what() const noexcept {
		return "VirusNotFound";
	}
};

class TriedToRemoveStemVirus : public std::exception {
public:
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

	VirusGenealogy(typename Virus::id_type const &stem_id) : stem_id(stem_id) {
		node_shared_ptr node_ptr(new VirusNode(stem_id, &graph));
		std::pair<virus_graph_iterator, bool> result = graph.insert(make_pair(stem_id, node_ptr.get()));
		node_ptr->set_position(result.first);
		stem_node_ptr = node_ptr;
	};

	typename Virus::id_type get_stem_id() const {
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

		if (parent_ids.empty())
			throw VirusNotFound();

		// Dziecko otrzymuje wskaźniki na swoich poprzedników.
		node_shared_ptr node_ptr(new VirusNode(id, &graph));

		for (auto &parent_id : parent_ids) {
			auto tmp = graph.find(parent_id);

			if (tmp == graph.end())
				throw VirusNotFound();

			node_ptr->parents.insert(tmp->second);
		}

		// Do tego momentu wierzchołek nie ma jeszcze żadnych powiązań
		// z grafem, zatem jeśli coś pójdzie nie tak, zostanie usunięty.
		// =============================================================
		// Początek modyfikacji zawartości grafu. Jeśli zostanie
		// rzucony wyjątek, modyfikacja zostanie cofnięta.

		std::pair<virus_graph_iterator, bool> result = graph.insert(make_pair(id, node_ptr.get()));
		node_ptr->set_position(result.first);

		try {
			for (auto parent_ptr : node_ptr->parents) {
				parent_ptr->children.insert(node_ptr);
			}
		}
		catch (...) {
			for (auto parent_ptr : node_ptr->parents) {
				parent_ptr->children.erase(node_ptr);
			}
			throw;
			// Rzucamy aktywny wyjątek dalej.
			// Jeśli wystąpił błąd, wierzchołek zostanie
			// usunięty z mapy przez swój destruktor.
		}
	}

	void connect(typename Virus::id_type const &child_id, typename Virus::id_type const &parent_id) {
		if (!exists(child_id) || !exists(parent_id))
			throw VirusNotFound();

		VirusNode * child  = graph.find( child_id)->second;
		VirusNode * parent = graph.find(parent_id)->second;

		node_shared_ptr child_ptr = child->shared_from_this();

		parent->children.insert(child_ptr);

		try {
			child->parents.insert(parent);
		}
		catch (...) {
			parent->children.erase(child_ptr);
			throw;
		}
	}

	void remove(typename Virus::id_type const &id) {
		if (!exists(id))
			throw VirusNotFound();

		if (id == stem_id)
			throw TriedToRemoveStemVirus();

		// Usuwam ten wskaźnik ze wszystkich rodziców, będzie to w konsekwencji jedyny shared_ptr
		// wskazujący na zadany wierzchołek. Zatem po zakonczeniu wywołania tej procedury wierzcholek
		// zostanie usunięty.
		node_shared_ptr node = graph.find(id)->second->shared_from_this();

		for (auto &parent : node->parents) {
			// Usuwanie jest noexcept:
			parent->children.erase(node);
		}
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

	class VirusNode : public std::enable_shared_from_this<VirusNode> {
	public:
		std::set<node_shared_ptr> children;
		std::set<VirusNode *>     parents;

		virus_graph * containing_graph;
		virus_graph_iterator position;
		bool is_position_valid;

		typename Virus::id_type id;
		Virus my_virus;

		VirusNode(typename Virus::id_type id, virus_graph * g) :
			containing_graph(g),
			is_position_valid(false),
			id(id),
			my_virus(Virus(id))
		{ }

		~VirusNode() noexcept {
			if (containing_graph != nullptr && is_position_valid)
				containing_graph->erase(position);
		}

		void set_position(virus_graph_iterator const &it) {
			position = it;
			is_position_valid = true;
		}
	};
};

#endif