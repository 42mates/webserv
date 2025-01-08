#### 1. INITIALISATION DU SERVEUR  

**a.** Chargement de la configuration :  
- Lire un fichier de configuration (chemin, ports, routes, etc.).
- Stocker les paramètres dans des structures de données (par ex., dictionnaire ou objets C++).  

**b.** Création des sockets d'écoute :  
- Pour chaque port configuré :  
	- `socket()` : Créer un socket.
	- `bind()` : Associer le socket à une adresse IP et un port.
	- `listen()` : Configurer le socket pour écouter les connexions entrantes.  

**c.** Passage en mode non-bloquant :  
- Configurer les sockets pour qu'ils soient non-bloquants (avec `fcntl()` ou équivalent).

---

#### 2. BOUCLE PRINCIPALE (EVENT LOOP)  

**a.** Préparation du gestionnaire d'événements :  
- Utiliser `poll()`, `epoll()`, ou `kqueue()` pour surveiller :  
	- Les sockets d'écoute (nouvelles connexions).
	- Les sockets clients (données à lire ou écrire).  

**b.** Boucle infinie :  
- Appeler `poll()` (ou équivalent) pour détecter les événements.
- Traiter chaque événement détecté.  

---

#### 3. GESTION DES CONNEXIONS ENTRANTES  

**a.** Accepter une nouvelle connexion :  
- Si un socket d'écoute est prêt :  
	- `accept()` pour créer un socket client.
	- Ajouter le socket client à `poll()` pour surveiller ses activités.

---

#### 4. TRAITEMENT DES REQUÊTES CLIENTS  

**a.** Lire les données :  
- Si un socket client est prêt à lire :  
	- `recv()` pour récupérer les données brutes.  

**b.** Analyser la requête HTTP :  
- Décomposer les données en :  
	- Ligne de requête (méthode, chemin, version).
	- Headers (Host, User-Agent, etc.).
	- Corps (pour les POST ou PUT).
- Valider et gérer les erreurs (400, 405, etc.).  

**c.** Déterminer l'action à effectuer :  
- Trouver la route correspondante dans la configuration.
- Décider de servir un fichier, exécuter un CGI, ou retourner une erreur.

---

#### 5. GÉNÉRATION DE LA RÉPONSE  

**a.** Construire une réponse HTTP :  
- Ligne de statut (ex. `HTTP/1.1 200 OK`).
- Headers (Content-Type, Content-Length, etc.).
- Corps (contenu HTML, JSON, etc.).  

**b.** Envoyer la réponse :  
- Utiliser `send()` pour transmettre les données au client.  

**c.** Gérer les connexions persistantes :  
- Si `Connection: keep-alive`, garder le socket ouvert.
- Sinon, fermer le socket après la réponse.

---

#### 6. GESTION DES CGI (SI NÉCESSAIRE)  

**a.** Préparer l'environnement CGI :  
- Créer les variables d'environnement (PATH_INFO, QUERY_STRING, etc.).
- Passer les données de la requête au programme CGI via `stdin` ou fichiers temporaires.  

**b.** Exécuter le programme CGI :  
- Utiliser `fork()` + `execve()` pour lancer le programme.  

**c.** Lire la sortie du CGI :  
- Récupérer les données générées par le CGI via un pipe ou fichier temporaire.  

**d.** Intégrer la sortie dans la réponse HTTP.

---

#### 7. NETTOYAGE ET ERREURS  

**a.** Gestion des erreurs :  
- Fournir des réponses HTTP précises (404, 500, etc.).
- Afficher une page d'erreur par défaut si nécessaire.  

**b.** Nettoyer les connexions fermées :  
- Supprimer les sockets clients inactifs ou ayant expiré de `poll()`.

---

#### 8. FERMETURE DU SERVEUR (SI ARRÊT)  

**a.** Fermer tous les sockets ouverts.  

**b.** Libérer les ressources (mémoire, fichiers temporaires, etc.).  

**c.** Arrêter proprement le programme.

