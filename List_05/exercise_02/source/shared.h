#ifndef EXERCISE_LIST_OPERATIONAL_SYSTEMS_SHARED_H
#define EXERCISE_LIST_OPERATIONAL_SYSTEMS_SHARED_H

struct Message {
    int id;
};

#define TRUE 1
#define FALSE 0

int produce_item();

void consume_item(int item);

void receive(mqd_t mq, struct Message *message);

int extract_item(struct Message *message);

void build_message(struct Message *message, int item);

void send(mqd_t mq, struct Message *message);

#endif //EXERCISE_LIST_OPERATIONAL_SYSTEMS_SHARED_H
