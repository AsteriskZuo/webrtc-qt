#ifndef PEERTERMINAL_H
#define PEERTERMINAL_H


#include "../libjingle_app/p2pconductor.h"
//#include "talk/app/webrtc/proxy.h"
#include "talk/base/buffer.h"
#include "../libjingle_app/PeerConnectionClinetInterface.h"
#include <vector>
//one terminal contains one PeerConnectionClient and some P2PConductor


class PeerTerminalInterface{
public:
    virtual int ConnectToPeer(const std::string &peer_id) = 0;
    virtual int CloseTunnel(const std::string &peer_id) = 0;
    virtual int SendByRouter(const std::string & peer_id,const std::string & data) =0;
    virtual int SendByTunnel(const std::string &peer_id,const std::string & data) =0;
    virtual int SendByTunnel(const std::string &peer_id,const char *data,size_t len) =0 ;
    sigslot::signal2<PeerTerminalInterface *,const std::string &> SignalTunnelOpened;
    sigslot::signal2<PeerTerminalInterface *,const std::string &> SignalTunnelClosed;

    sigslot::signal2<const std::string &,talk_base::Buffer &> SignalTunnelMessage;
    sigslot::signal2<const std::string &,const std::string &> SignalRouterMessage;


};
typedef talk_base::scoped_refptr<kaerp2p::P2PConductor> ScopedTunnel;


class PeerTerminal:public PeerTerminalInterface, public sigslot::has_slots<>
{
public:
    PeerTerminal();
    //need a client that has been login
    int Initialize(kaerp2p::PeerConnectionClientInterface * client);

    virtual int ConnectToPeer(const std::string &peer_id);
    virtual int CloseTunnel(const std::string &peer_id);
    //virtual bool TunnelOpened();

    virtual int SendByRouter(const std::string & peer_id,const std::string & data);
    virtual int SendByTunnel(const std::string &peer_id,const std::string & data);
    virtual int SendByTunnel(const std::string &peer_id,const char *data,size_t len);

    //int AskTunnelVideo();
protected:
    void OnTunnelOpened(kaerp2p::StreamProcess * stream);
    void OnTunnelClosed(kaerp2p::StreamProcess * tunnel);
    void OnTunnelReadData(kaerp2p::StreamProcess * stream,size_t len);
    void OnRouterReadData(const std::string & peer_id, const std::string & msg);
    void OnTunnelNeedSend(const std::string& peer_id, const std::string& msg);
private:
    ScopedTunnel GetTunnel(const std::string & peer_id);
    ScopedTunnel GetTunnel(kaerp2p::StreamProcess * stream);
    int CountAvailableTunnels();
    ScopedTunnel GetOrCreateTunnel(const std::string & peer_id);

    kaerp2p::PeerConnectionClientInterface * client_;
    //talk_base::scoped_refptr<kaerp2p::P2PConductor> conductor_;
    std::vector<ScopedTunnel> tunnels_;
    int max_tunnel_num_;

};

#endif // PEERTERMINAL_H