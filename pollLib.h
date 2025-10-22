#ifndef __POLLLIB_H__
#define __POLLLIB_H__

#define POLL_SET_SIZE 10
#define POLL_WAIT_FOREVER -1

void setupPollSet();
void addToPollSet(int socketNumber);
void removeFromPollSet(int socketNumber);
int pollCall(int timeInMilliSeconds);

#endif