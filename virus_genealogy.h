#ifndef _VIRUS_GENEALOGY_
#define _VIRUS_GENEALOGY_

#include <exception>
#include <map>
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

public:

	VirusGenealogy(typename Virus::id_type const &stem_id)
		: stem_id(stem_id)
	{ };

	typename Virus::id_type get_stem_id() const noexcept {
		return stem_id;
	}

	std::vector<typename Virus::id_type> get_children(typename Virus::id_type const &id) const {
		// TODO
		std::vector<typename Virus::id_type> result;
		return result;
	}

	std::vector<typename Virus::id_type> get_parents (typename Virus::id_type const &id) const {
		// TODO
		std::vector<typename Virus::id_type> result;
		return result;
	}

	bool exists(typename Virus::id_type const &id) const;
	Virus& operator[](typename Virus::id_type const &id) const;

	void create(typename Virus::id_type const &id, typename Virus::id_type const &parent_id) {
		virus_graph_iterator parent_it = graph.find(parent_id);

		if (parent_it == graph.end())
			throw VirusNotFound();

		if (graph.find(id) != graph.end())
			throw VirusAlreadyCreated();
	
		std::shared_ptr<VirusNode> node_ptr(new VirusNode(id, &graph));

		// Początek modyfikacji zawartości grafu. Jeśli zostanie
		// rzucony wyjątek, modyfikacja nie zostanie wprowadzona.

		virus_graph_iterator position = graph.insert(make_pair(id, &(*node_ptr)));

		try {
			node_ptr->set_position(position);

			(*parent_it).children.push_back(ChildPtr(node_ptr));
		}
		catch (...) {
			// Jeśli nie udało się ustawić pozycji lub zapisać wskaźnika w ojcu wierzchołka,
			// jedynym obiektem shared_ptr wskazującym na nowo utworzony wierzchołek będzie
			// node_ptr, który jest lokalny, zatem po zakończeniu wywołania funkcji zostanie
			// usunięty.
			graph.erase(position);
		}
	}

	void create(typename Virus::id_type const &id, std::vector<typename Virus::id_type> const &parent_ids);

	void connect(typename Virus::id_type const &child_id, typename Virus::id_type const &parent_id);
	void remove(typename Virus::id_type const &id) {


	}

private:

	virus_graph graph;
	typename Virus::id_type stem_id;
	
	// Konstruktor kopiujący oraz operator przypisania mają być niedostępne:
	VirusGenealogy(VirusGenealogy<Virus> const &) = delete;
	VirusGenealogy& operator=(VirusGenealogy<Virus> const &) = delete;

	//
	// Klasa reprezentująca wierzchołek w grafie
	//

	class VirusNode {
	public:

		typedef std::shared_ptr<VirusNode>  ChildPtr;
		typedef std::  weak_ptr<VirusNode> ParentPtr;

		std::vector<ChildPtr> children;
		std::vector<ParentPtr> parents;

		virus_graph * containing_graph;
		virus_graph_iterator position;

		Virus my_virus;

		// typename Virus::id_type id;

		VirusNode(typename Virus::id_type id, virus_graph * g) :
			containing_graph(g),
			my_virus(Virus(id))
		{ }

		~VirusNode() {
			if (*containing_graph != nullptr)
				*containing_graph.erase(position);
		}

		void set_position(virus_graph_iterator const &it) {
			position = it;
		}

		void connect_parent(typename Virus::id_type &parent_id) {
			parents.push_back(ParentPtr(*containing_graph[parent_id]));
		}

		void connect_child(typename Virus::id_type &child_id) {
			children.push_back(ChildPtr(*containing_graph[child_id]));
		}
	};
};













//
// Implementacja metod klasy VirusGenealogy
//

// Tworzy nową genealogię.
// Tworzy także węzęł wirusa macierzystego o identyfikatorze stem_id.
// template <class Virus>
// VirusGenealogy<Virus>::VirusGenealogy(typename Virus::id_type const &stem_id) : stem_id(stem_id) {

// 	// graph.insert()
// 	// std::cerr << "Create genealogy with stem_id = " << stem_id << std::endl;
	
// }

// Zwraca identyfikator wirusa macierzystego.
// template <class Virus>
// typename Virus::id_type VirusGenealogy<Virus>::get_stem_id() const noexcept {
// 	return stem_id;
// }

// Zwraca listę identyfikatorów bezpośrednich następników wirusa
// o podanym identyfikatorze.
// Zgłasza wyjątek VirusNotFound, jeśli dany wirus nie istnieje.
// template <class Virus>
// std::vector<typename Virus::id_type> VirusGenealogy<Virus>::get_children(typename Virus::id_type const &id) const {
// 	std::vector<typename Virus::id_type> result;
// 	return result;
// }

// Zwraca listę identyfikatorów bezpośrednich poprzedników wirusa
// o podanym identyfikatorze.
// Zgłasza wyjątek VirusNotFound, jeśli dany wirus nie istnieje.
/*template <class Virus>
std::vector<typename Virus::id_type> VirusGenealogy<Virus>::get_parents(typename Virus::id_type const &id) const {

}

// Sprawdza, czy wirus o podanym identyfikatorze istnieje.
template <class Virus>
bool VirusGenealogy<Virus>::exists(typename Virus::id_type const &id) const {

}

// Zwraca referencję do obiektu reprezentującego wirus o podanym
// identyfikatorze.
// Zgłasza wyjątek VirusNotFound, jeśli żądany wirus nie istnieje.
template <class Virus>
Virus& VirusGenealogy<Virus>::operator[](typename Virus::id_type const &id) const {

}

// Tworzy węzęł reprezentujący nowy wirus o identyfikatorze id
// powstały z wirusów o podanym identyfikatorze parent_id lub
// podanych identyfikatorach parent_ids.
// Zgłasza wyjątek VirusAlreadyCreated, jeśli wirus o identyfikatorze
// id już istnieje.
// Zgłasza wyjątek VirusNotFound, jeśli któryś z wyspecyfikowanych
// poprzedników nie istnieje.*/
// template <class Virus>
// void VirusGenealogy<Virus>::create(typename Virus::id_type const &id, typename Virus::id_type const &parent_id) {
// 	shared_ptr<typename VirusGenealogy<Virus>::VirusNode> node_ptr;

// 	try {
// 		// Podczas sprawdzania istnienia wierzchołka mogą być rzucone wyjątki.

// 		VirusGenealogy<Virus>::virus_graph_iterator parent_it = graph.find(parent_id);

// 		if (parent_it == graph.end())
// 			throw VirusNotFound();

// 		if (graph.find(id) != graph.end())
// 			throw VirusAlreadyCreated();

// 		graph[id] = node_ptr = new VirusGenealogy<Virus>::VirusNode(id, *graph);



// 		// if 
// 		// VirusGenealogy<Virus>::VirusNode& parent_node = graph[parent_id];

// 		// shared_ptr<VirusGenealogy<Virus>::VirusNode> node_ptr(new VirusGenealogy<Virus>::VirusNode(id, this));
// 		// graph[id] = *node_ptr;
		
// 	}
// 	// Należy zauważyć, że nie ma możliwości rozróżnienia wyjątków, które
// 	// byłyby rzucane wewnątrz bloku try tej funkcji od wyjątków rzucanych
// 	// przez operatory porównania typu Virus::id_type. Jest tak dlatego, że
// 	// klasa Virus w teorii może rzucać wyjątki zdefiniowane w tym pliku
// 	// nagłównowym (np. VirusNotFound). Dlatego nie ma możliwości zastosowania
// 	// rozwiązania polegającego na łapaniu i dalszym rzucaniu tych wyjątków.
// 	// (Wyjątki rzucone przez operatory porównania muszą być złapane - inne nie)
// 	catch (VirusNotFound       const &e) { throw e; }
// 	catch (VirusAlreadyCreated const &e) { throw e; }
// 	// Inne wyjątki łapiemy i cofamy modyfikacje struktury danych.
// 	catch (...) {

// 	}
// }

/*template <class Virus>
void VirusGenealogy<Virus>::create(typename Virus::id_type const &id, std::vector<typename Virus::id_type> const &parent_ids) {
	shared_ptr<typename VirusGenealogy<Virus>::VirusNode> node_ptr(new VirusGenealogy<Virus>::VirusNode(id, id));

}*/
/*
// Dodaje nową krawędź w grafie genealogii.
// Zgłasza wyjątek VirusNotFound, jeśli któryś z podanych wirusów nie istnieje.
template <class Virus>
void VirusGenealogy<Virus>::connect(typename Virus::id_type const &child_id, typename Virus::id_type const &parent_id) {

}

// Usuwa wirus o podanym identyfikatorze.
// Zgłasza wyjątek VirusNotFound, jeśli żądany wirus nie istnieje.
// Zgłasza wyjątek TriedToRemoveStemVirus przy próbie usunięcia
// wirusa macierzystego.
template <class Virus>
void VirusGenealogy<Virus>::remove(typename Virus::id_type const &id) {

}*/

#endif