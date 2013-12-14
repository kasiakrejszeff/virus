#ifndef _VIRUS_GENEALOGY_
#define _VIRUS_GENEALOGY_

template <class Virus>
class VirusGenealogy {
public:
	// Tworzy nową genealogię.
	// Tworzy także węzęł wirusa macierzystego o identyfikatorze stem_id.
	VirusGenealogy(typename Virus::id_type const &stem_id);

	// Zwraca identyfikator wirusa macierzystego.
	typename Virus::id_type get_stem_id() const;

	// Zwraca listę identyfikatorów bezpośrednich następników wirusa
	// o podanym identyfikatorze.
	// Zgłasza wyjątek VirusNotFound, jeśli dany wirus nie istnieje.
	std::vector<typename Virus::id_type> get_children(typename Virus::id_type const &id) const;

	// Zwraca listę identyfikatorów bezpośrednich poprzedników wirusa
	// o podanym identyfikatorze.
	// Zgłasza wyjątek VirusNotFound, jeśli dany wirus nie istnieje.
	std::vector<typename Virus::id_type> get_parents(typename Virus::id_type const &id) const;

	// Sprawdza, czy wirus o podanym identyfikatorze istnieje.
	bool exists(typename Virus::id_type const &id) const;

	// Zwraca referencję do obiektu reprezentującego wirus o podanym
	// identyfikatorze.
	// Zgłasza wyjątek VirusNotFound, jeśli żądany wirus nie istnieje.
	Virus& operator[](typename Virus::id_type const &id) const;

	// Tworzy węzęł reprezentujący nowy wirus o identyfikatorze id
	// powstały z wirusów o podanym identyfikatorze parent_id lub
	// podanych identyfikatorach parent_ids.
	// Zgłasza wyjątek VirusAlreadyCreated, jeśli wirus o identyfikatorze
	// id już istnieje.
	// Zgłasza wyjątek VirusNotFound, jeśli któryś z wyspecyfikowanych
	// poprzedników nie istnieje.
	void create(typename Virus::id_type const &id, typename Virus::id_type const &parent_id);
	void create(typename Virus::id_type const &id, std::vector<typename Virus::id_type> const &parent_ids);

	// Dodaje nową krawędź w grafie genealogii.
	// Zgłasza wyjątek VirusNotFound, jeśli któryś z podanych wirusów nie istnieje.
	void connect(typename Virus::id_type const &child_id, typename Virus::id_type const &parent_id);

	// Usuwa wirus o podanym identyfikatorze.
	// Zgłasza wyjątek VirusNotFound, jeśli żądany wirus nie istnieje.
	// Zgłasza wyjątek TriedToRemoveStemVirus przy próbie usunięcia
	// wirusa macierzystego.
	void remove(typename Virus::id_type const &id);

private:

};

template <class Virus>
VirusGenealogy<Virus>::VirusGenealogy(typename Virus::id_type const &stem_id) {
	// TODO
}

#endif