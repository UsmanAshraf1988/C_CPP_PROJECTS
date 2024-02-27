#include <iostream>
#include <functional>
#include <vector>
#include <map>

// Custom Signal class
class SignalSlot{
public:
    virtual void link(std::string str, std::function<void()> slot_)= 0;
    virtual void emit(std::string str) = 0;
    
    static void connect(std::string str, SignalSlot * pSrc, std::function<void()> slot_)
    {
        if(pSrc){
            pSrc->link(str, slot_);
        }
    }
};

class Signal: public SignalSlot {
public:
    void link(std::string str, std::function<void()> slot_) override {
        connector[str].push_back(slot_);
    }

    void emit(std::string str) override {
        for (const auto& slot : connector[str]) {
            slot();
        }
    }
    
    virtual ~Signal(){
        for(auto it=connector.begin(); it!=connector.end(); ++it){
            std::cout << std::hex << this << " ==> "<< it->first << ": "<< it->second.size() << std::endl;
        }
    }

private:
    std::map< std::string , std::vector<std::function<void()>> > connector;
};

class BaseObject : public Signal { };

class MediaManager : public BaseObject { 
public:
    void setPrint(){
        emit("MediaManager::setPrint");
    }
    void setMessage(){
        emit("MediaManager::setMessage");
    }
};

class ProgressConsole : public BaseObject { 
public:
    void showMessage(){
        std::cout << std::hex << this << " ==> I am a showMessage using Signal and Slot\n";
    }
    void showPrint(){
        std::cout << std::hex << this << " ==> I am a showPrint using Signal and Slot\n";
    }
};


int main() {
    MediaManager mediaManager;
    ProgressConsole progressConsole;
    SignalSlot::connect("MediaManager::setMessage", &mediaManager, std::bind (&ProgressConsole::showMessage, &progressConsole));
    SignalSlot::connect("MediaManager::setPrint", &mediaManager, std::bind (&ProgressConsole::showPrint, &progressConsole) );    
    mediaManager.setMessage();
    mediaManager.setPrint();
    
    MediaManager mediaManager2;
    ProgressConsole progressConsole2;
    SignalSlot::connect("MediaManager::setMessage", &mediaManager2, std::bind (&ProgressConsole::showMessage, &progressConsole2));
    SignalSlot::connect("MediaManager::setPrint", &mediaManager2, std::bind (&ProgressConsole::showPrint, &progressConsole2) ); 
    mediaManager2.setMessage();
    mediaManager2.setPrint();
    
    return 0;
}
