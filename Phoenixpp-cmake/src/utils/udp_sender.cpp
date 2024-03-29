#include "udp_sender.h"

UdpSender::UdpSender(atomic<bool>& stop, queue<pair<char*,int>> &bufferQueue, mutex &bufferQueue_mtx,
                         string multicastAddress, string port, int bufferSizeMax, int queueSizeMax) :
    stop(stop), bufferQueue(bufferQueue), bufferQueue_mtx(bufferQueue_mtx), socket(Poco::Net::MulticastSocket()),
    transmitted(false), multicastAddress(multicastAddress), port(port), stopSender(false),
    bufferSizeMax(bufferSizeMax), queueSizeMax(queueSizeMax) {
}

void UdpSender::start(){
    //reconnect();
    /*
    Poco::Net::ReadableNotification readableNotification(&reactor);
    reactor.addEventHandler(socket, Poco::Observer<UdpSender, Poco::Net::ReadableNotification>(*this, &UdpSender::onReadable));
    reactor.addEventHandler(socket, Poco::Observer<UdpSender, Poco::Net::TimeoutNotification>(*this, &UdpSender::onTimeout));
    reactor.setTimeout(Poco::Timespan(chrono::milliseconds(500)));
    */
    receiveThread = thread([this]{ sendPacket(); });
    //reactorThread = thread([this]{ reactor.run(); });
}

void UdpSender::reconnect(){
    try{
        Poco::Net::SocketAddress group(multicastAddress, port);
        //socket.bind(Poco::Net::SocketAddress("0.0.0.0", port));
        //socket.connect(group);
        //socket.setReceiveTimeout(Poco::Timespan(1, 0));
    }
    catch (Poco::Exception& e) {
        cerr << "Error trying to reconnect to multicast group: " << e.displayText() << endl;
    }
}

void UdpSender::sendPacket() {
    Poco::Net::SocketAddress group(multicastAddress, port);
    Poco::Net::SocketImpl* socketImpl;
    pair<char*, int> front;
    int size;
    while (!stop.load() && !stopSender.load()) {
        {
            unique_lock<mutex> lock(bufferQueue_mtx);
            //cerr << "send queue size: " << bufferQueue.size() << "\n";
            if (bufferQueue.empty()) continue;
            front = bufferQueue.front();
            buffer = front.first;
            size = front.second;
            //for(int i=0;i<size;i++) cout << i << ": " << (int)buffer[i] << endl;
            //cout << endl;
            bufferQueue.pop();
        }
        try{
            socket.sendTo(buffer, size, group);
            transmitted.store(true);
            //cerr << "received\n";
        } catch (const Poco::Exception& e) {
            transmitted.store(false);
        }
        delete [] buffer; // deletando espaco alocado pelo udpCommunicator ao criar a mensagem
    }
    socket.close();
    //this_thread::sleep_for(chrono::seconds(500));
}

bool UdpSender::getReceived(){
    return transmitted.load();
}

UdpSender::~UdpSender(){
    cout << "destruindo udp Sender" << endl;
    stopSender.store(true);
    if(receiveThread.joinable()) receiveThread.join();
    //if(reactorThread.joinable()) reactorThread.join();
    cout << "done udp Sender" << endl;
}
