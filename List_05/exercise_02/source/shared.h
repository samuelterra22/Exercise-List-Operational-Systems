#ifndef EXERCISE_LIST_OPERATIONAL_SYSTEMS_SHARED_H
#define EXERCISE_LIST_OPERATIONAL_SYSTEMS_SHARED_H

struct Message {
    int id;
    struct inotify_event *event;
};

#define TRUE 1
#define FALSE 0

#define MQ_NAME_WATCHER "/mq_watcher"

int produce_item();

void consume_item(int item);

void receive(mqd_t mq, struct Message *message);

struct inotify_event* extract_item(struct Message *message);

void build_message(struct Message *message, struct inotify_event *event);

void send(mqd_t mq, struct Message *message);

void display_inotify_event(struct inotify_event *i);

#endif //EXERCISE_LIST_OPERATIONAL_SYSTEMS_SHARED_H
