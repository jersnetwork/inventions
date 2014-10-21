


#ifndef _STACKSIM_H_
#define _STACKSIM_H_
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <utility>
#include "packet.h"

bool less_than(const tcp_packet& a, const tcp_packet& b) {
  return a.get_seq() < b.get_seq();
}

class tcp_stack {
 public:
  tcp_stack();
  std::string get_payload(std::string connection);
  std::string get_payload(int index);
  int num_connections();
  void list_streams();
  void insert(tcp_packet packet);

 private:
  std::map<std::string, std::vector<tcp_packet> > streams;
};

tcp_stack::tcp_stack() {
  streams.clear();
}

std::string tcp_stack::get_payload(std::string connection) {
  std::string payload;
  std::vector<tcp_packet>::iterator itr;

  if (!connection.empty()) {
    std::string::size_type pos = connection.find_last_of('\n');
    connection.erase(pos);
  }

  std::sort(streams[connection].begin(), streams[connection].end(), less_than);

  for (itr = streams[connection].begin(); itr != streams[connection].end(); itr++)
    payload += itr->get_data();

  return payload;
}

std::string tcp_stack::get_payload(int index) {
  std::string payload;
  std::map<std::string, std::vector<tcp_packet> >::iterator mitr = streams.begin();
  std::vector<tcp_packet>::iterator pitr;

  if(index > streams.size())
    throw "out of bounds";
  index -= 1;

  std::advance(mitr, index);
  std::sort(mitr->second.begin(), mitr->second.end(), less_than);

  for (pitr = mitr->second.begin(); pitr != mitr->second.end(); pitr++)
    payload += pitr->get_data();

  return payload;
}

int tcp_stack::num_connections() {
  return streams.size();
}

void tcp_stack::insert(tcp_packet p) {
  std::ostringstream connection;
  connection << p.get_src_ip()
             << ":"
             << p.get_src_port()
             << " > "
             << p.get_dst_ip()
             << ":"
             << p.get_dst_port();

  if (streams.find(connection.str()) == streams.end()) {
    // not found
    std::pair<std::string, std::vector<tcp_packet> >
      stream(connection.str(), std::vector<tcp_packet>());
    streams.insert(stream);

  }
  // check for duplicates/retransmissions.
  // overwrite instead of push back to accomodate for phantoms
  std::vector<tcp_packet>::iterator itr;
  for(itr = streams[connection.str()].begin(); itr != streams[connection.str()].end(); itr++)
    if(itr->get_seq() == p.get_seq())
      *itr = p;

  if(itr == streams[connection.str()].end())
    streams[connection.str()].push_back(p);
}

void tcp_stack::list_streams() {
  std::map<std::string, std::vector<tcp_packet> >::iterator itr;

  for(itr = streams.begin(); itr != streams.end(); itr++){
    std::cout << std::distance(streams.begin(), itr) + 1
              << ") "
              << itr->first << std::endl;
  }
}
#endif //_STACKSIM_H_
