#ifndef _VIRUS_GENEALOGY_
#define _VIRUS_GENEALOGY_

#include "virus.h"

template <class Virus>
class VirusGenealogy {
	// Tworzy nową genealogię.
	// Tworzy także węzęł wirusa macierzystego o identyfikatorze stem_id.
	VirusGenealogy(typename Virus::id_type const &stem_id);

};

#endif