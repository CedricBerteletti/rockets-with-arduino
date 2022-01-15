/*
 * --------------------------------------------------------------------------
 * Quelques fonctions utiles sur les chaînes de caractères
 * --------------------------------------------------------------------------
 * @author Cédric Berteletti
 */

#include "aides.hpp"

// Formatage des log
void formatLog(char destination[], const char module[], const char message[], const char details[]) {
  sprintf(destination, "%i, %s, \"%s\", \"%s\"", millis(), module, message, details);
}

// Teste si une chaîne de caractères commence par un prefix donné
bool chaineCommencePar(const char chaine[], const char prefix[])
{
    return strncmp(prefix, chaine, strlen(prefix)) == 0;
}

// Teste si une chaîne de caractères contient une sous-chaîne
bool chaineContient(const char chaine[], const char sousChaine[]) {
  return strstr(chaine, sousChaine) != NULL;
}

// Supprime les espaces au début et à la fin de la chaîne de caractères
// https://stackoverflow.com/questions/122616/how-do-i-trim-leading-trailing-whitespace-in-a-standard-way
char *trim(char *str)
{
    size_t len = 0;
    char *frontp = str;
    char *endp = NULL;

    if( str == NULL ) { return NULL; }
    if( str[0] == '\0' ) { return str; }

    len = strlen(str);
    endp = str + len;

    /* Move the front and back pointers to address the first non-whitespace
     * characters from each end.
     */
    while( isspace((unsigned char) *frontp) ) { ++frontp; }
    if( endp != frontp )
    {
        while( isspace((unsigned char) *(--endp)) && endp != frontp ) {}
    }

    if( frontp != str && endp == frontp )
            *str = '\0';
    else if( str + len - 1 != endp )
            *(endp + 1) = '\0';

    /* Shift the string so that it starts at str so that if it's dynamically
     * allocated, we can still free it on the returned pointer.  Note the reuse
     * of endp to mean the front of the string buffer now.
     */
    endp = str;
    if( frontp != str )
    {
            while( *frontp ) { *endp++ = *frontp++; }
            *endp = '\0';
    }

    return str;
}

// Copie une sous-chaîne donnée de la chaîne principale, comprise entre des séparateurs.
void copierToken(const char *chaine, const char separateur[], int index, char *token, bool dernierToken) {
  char *debut = (char*)chaine;
  char *fin;
  int i = 0;
  int tailleSeparateur = strlen(separateur);

  // Trouver le début du index-ième token
  while (i < index && debut != NULL) {
    debut = strstr(debut, separateur);
    if (debut != NULL) debut = debut + tailleSeparateur;
    i++;
  }
  if(debut == NULL && i > 0) {
    // token non trouvé
    token[0] = 0;
  }
  else{    
    if (debut == NULL) debut = (char*)chaine;

    if(dernierToken) {
      strcpy(token, debut);
    }
    else {
      // Copier jusqu'au token suivant (ou la fin)
      fin = strstr(debut, separateur);
      if (fin == NULL) fin = debut + strlen(debut);
      int taille = fin-debut;
      strncpy(token, debut, taille);
      // Caractère 0 pour terminer la chaîne
      token[taille] = NULL;
    }
  }
}

void copierToken(const char *chaine, const char separateur[], int index, char *token) {
  copierToken(chaine, separateur, index, token, false);
}