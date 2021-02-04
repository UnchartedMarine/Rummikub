bool verif_plateau(PLATEAU * plateau)
{
	ELEMENT_PLATEAU* element =  plateau->premier;

	while(element != NULL)
	{
		if(!est_suite(element->liste) && !est_liste_meme_nb(element->liste))
			return false;
		element=element->suivant;
	}

	return true;
}

