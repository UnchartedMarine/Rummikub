bool joker_est_dans_liste(LISTE * liste)
{
	ELEMENT * element=liste->premier;
	
	while(element != NULL)
	{
		if(element->tuile.num == 0)
			return true;
		element=element->suivant;
	}
	return false;
}
