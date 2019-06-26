#include "diffie_hellman.h"

class Server {
 private:
  long long private_key[MAX_CONN];
  long long p[MAX_CONN];
  long long g[MAX_CONN];

  int server_socket;
  int nr_of_clients = 0;
  pthread_t clients_threads[MAX_CONN];
  int clients_sockets[MAX_CONN];
  pthread_mutex_t accept_mutex = PTHREAD_MUTEX_INITIALIZER;

  void serverLoopToAcceptConnections();
  
 public:
  void startServer();
};
