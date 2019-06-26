#include "diffie_hellman.h"

class Client {
 private:
  long long private_key;
  long long p;
  long long g;

  string ipaddr;
  int client_socket;

  void keyExchange();

 public:
  Client(string);
  Client(long long, long long, string);

  void startConnection();
};
