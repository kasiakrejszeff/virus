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
public:
	VirusGenealogy(typename Virus::id_type const &stem_id);

	typename Virus::id_type get_stem_id() const noexcept;

	std::vector<typename Virus::id_type> get_children(typename Virus::id_type const &id) const;
	std::vector<typename Virus::id_type> get_parents (typename Virus::id_type const &id) const;

	bool exists(typename Virus::id_type const &id) const;
	Virus& operator[](typename Virus::id_type const &id) const;

	void create(typename Virus::id_type const &id, typename Virus::id_type const &parent_id);
	void create(typename Virus::id_type const &id, std::vector<typename Virus::id_type> const &parent_ids);

	void connect(typename Virus::id_type const &child_id, typename Virus::id_type const &parent_id);
	void remove(typename Virus::id_type const &id);

private:

	class VirusNode {
	public:
		typedef std::shared_ptr<VirusNode>  ChildPtr;
		typedef std::  weak_ptr<VirusNode> ParentPtr;

		std::vector<ChildPtr> children;
		std::vector<ParentPtr> parents;

		typename Virus::id_type id;

		// VirusNode()
	};

	typedef std::map<typename Virus::id_type, VirusNode *> virus_graph;

	virus_graph graph;
	typename Virus::id_type stem_id;

	
	// Konstruktor kopiujący oraz operator przypisania mają być niedostępne:
	VirusGenealogy(VirusGenealogy<Virus> const &) = delete;
	VirusGenealogy& operator=(VirusGenealogy<Virus> const &) = delete;

};

//
// Implementacja metod klasy VirusGenealogy
//

// Tworzy nową genealogię.
// Tworzy także węzęł wirusa macierzystego o identyfikatorze stem_id.
template <class Virus>
VirusGenealogy<Virus>::VirusGenealogy(typename Virus::id_type const &stem_id) : stem_id(stem_id) {
	// std::cerr << "Create genealogy with stem_id = " << stem_id << std::endl;
	
}

// Zwraca identyfikator wirusa macierzystego.
template <class Virus>
typename Virus::id_type VirusGenealogy<Virus>::get_stem_id() const noexcept {
	return stem_id;
}

// Zwraca listę identyfikatorów bezpośrednich następników wirusa
// o podanym identyfikatorze.
// Zgłasza wyjątek VirusNotFound, jeśli dany wirus nie istnieje.
/*template <class Virus>
std::vector<typename Virus::id_type> VirusGenealogy<Virus>::get_children(typename Virus::id_type const &id) const {

}

// Zwraca listę identyfikatorów bezpośrednich poprzedników wirusa
// o podanym identyfikatorze.
// Zgłasza wyjątek VirusNotFound, jeśli dany wirus nie istnieje.
template <class Virus>
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
// poprzedników nie istnieje.
template <class Virus>
void VirusGenealogy<Virus>::create(typename Virus::id_type const &id, typename Virus::id_type const &parent_id) {

}

template <class Virus>
void VirusGenealogy<Virus>::create(typename Virus::id_type const &id, std::vector<typename Virus::id_type> const &parent_ids) {

}

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