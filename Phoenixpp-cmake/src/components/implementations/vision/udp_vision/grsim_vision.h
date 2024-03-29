#ifndef UDP_VISION_H
#define UDP_VISION_H

#include <mutex>
#include <condition_variable>
#include <atomic>
#include <thread>
#include <iostream>
#include <memory>
#include <queue>
#include "../../../../../proto/protobuf_files.h"
#include "../../../../utils/udp_receiver.h"
#include "../../../base_component.h"
#include "../../../messages/environment.h"

class UdpVision : public TBaseComponent<Environment>{
private:
    shared_ptr<UdpReceiver> udpReceiver;
    queue<pair<char*,int>> bufferQueue;
    mutex bufferQueue_mtx;
public:
    UdpVision(std::atomic<bool>& stop, double fps);
    Environment update(Environment message) override;
    void start() override;
    ~UdpVision();
};

#endif // UDP_VISION_H
