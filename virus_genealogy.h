#ifndef _VIRUS_GENEALOGY_
#define _VIRUS_GENEALOGY_

template <class Virus>
class VirusGenealogy {
public:
	// Tworzy nową genealogię.
	// Tworzy także węzęł wirusa macierzystego o identyfikatorze stem_id.
	VirusGenealogy(typename Virus::id_type const &stem_id);

};


template <class Virus>
VirusGenealogy<Virus>::VirusGenealogy(typename Virus::id_type const &stem_id) {
	// TODO
}

#endif