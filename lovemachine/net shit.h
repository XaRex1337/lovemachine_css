#pragma once
#include "includes.h"

// thx catalinadragan22 (c) // uc

class INetChannelInfo
{
public:

	enum
	{
		GENERIC = 0,	// must be first and is default group
		LOCALPLAYER,	// bytes for local player entity update
		OTHERPLAYERS,	// bytes for other players update
		ENTITIES,		// all other entity bytes
		SOUNDS,			// game sounds
		EVENTS,			// event messages
		TEMPENTS,		// temp entities
		USERMESSAGES,	// user messages
		ENTMESSAGES,	// entity messages
		VOICE,			// voice data
		STRINGTABLE,	// a stringtable update
		MOVE,			// client move cmds
		STRINGCMD,		// string command
		SIGNON,			// various signondata
		TOTAL,			// must be last and is not a real group
	};

	virtual const char* GetName(void) const = 0;	// get channel name
	virtual const char* GetAddress(void) const = 0; // get channel IP address as string
	virtual float		GetTime(void) const = 0;	// current net time
	virtual float		GetTimeConnected(void) const = 0;	// get connection time in seconds
	virtual int			GetBufferSize(void) const = 0;	// netchannel packet history size
	virtual int			GetDataRate(void) const = 0; // send data rate in byte/sec

	virtual bool		IsLoopback(void) const = 0;	// true if loopback channel
	virtual bool		IsTimingOut(void) const = 0;	// true if timing out
	virtual bool		IsPlayback(void) const = 0;	// true if demo playback

	virtual float		GetLatency(int flow) const = 0;	 // current latency (RTT), more accurate but jittering
	virtual float		GetAvgLatency(int flow) const = 0; // average packet latency in seconds
	virtual float		GetAvgLoss(int flow) const = 0;	 // avg packet loss[0..1]
	virtual float		GetAvgChoke(int flow) const = 0;	 // avg packet choke[0..1]
	virtual float		GetAvgData(int flow) const = 0;	 // data flow in bytes/sec
	virtual float		GetAvgPackets(int flow) const = 0; // avg packets/sec
	virtual int			GetTotalData(int flow) const = 0;	 // total flow in/out in bytes
	virtual int			GetTotalPackets(int flow) const = 0;
	virtual int			GetSequenceNr(int flow) const = 0;	// last send seq number
	virtual bool		IsValidPacket(int flow, int frame_number) const = 0; // true if packet was not lost/dropped/chocked/flushed
	virtual float		GetPacketTime(int flow, int frame_number) const = 0; // time when packet was send
	virtual int			GetPacketBytes(int flow, int frame_number, int group) const = 0; // group size of this packet
	virtual bool		GetStreamProgress(int flow, int* received, int* total) const = 0;  // TCP progress if transmitting
	virtual float		GetTimeSinceLastReceived(void) const = 0;	// get time since last recieved packet in seconds
	virtual	float		GetCommandInterpolationAmount(int flow, int frame_number) const = 0;
	virtual void		GetPacketResponseLatency(int flow, int frame_number, int* pnLatencyMsecs, int* pnChoke) const = 0;
	virtual void		GetRemoteFramerate(float* pflFrameTime, float* pflFrameTimeStdDeviation, float* pflFrameStartTimeStdDeviation) const = 0;

	virtual float		GetTimeoutSeconds() const = 0;
};

class INetChannel : public INetChannelInfo
{
public:
	virtual ~INetChannel(void)
	{
	};

	virtual void SetDataRate(float rate) = 0;
	virtual bool RegisterMessage(void** msg) = 0; // void* => INetMessage
	virtual bool StartStreaming(unsigned int challengeNr) = 0;
	virtual void ResetStreaming(void) = 0;
	virtual void SetTimeout(float seconds) = 0;
	//virtual void SetDemoRecorder(IDemoRecorder *recorder) = 0;
	virtual void SetChallengeNr(unsigned int chnr) = 0;

	virtual void Reset(void) = 0;
	virtual void Clear(void) = 0;
	virtual void Shutdown(const char* reason) = 0;

	virtual void ProcessPlayback(void) = 0;
	virtual bool ProcessStream(void) = 0;
	virtual void ProcessPacket(struct netpacket_s* packet, bool bHasHeader) = 0;

	virtual bool SendNetMsg(void*& msg, bool bForceReliable = false, bool bVoice = false) = 0; // void* => INetMessage
#ifdef POSIX
	FORCEINLINE bool SendNetMsg(INetMessage const& msg, bool bForceReliable = false, bool bVoice = false)
	{
		return SendNetMsg(*((INetMessage*)&msg), bForceReliable, bVoice);
	}
#endif
	//virtual bool SendData(bf_write &msg, bool bReliable = true) = 0;
	virtual bool SendFile(const char* filename, unsigned int transferID) = 0;
	virtual void DenyFile(const char* filename, unsigned int transferID) = 0;
	virtual void RequestFile_OLD(const char* filename, unsigned int transferID) = 0; // get rid of this function when we version the
	virtual void SetChoked(void) = 0;
	//virtual int SendDatagram(bf_write *data) = 0;
	virtual bool Transmit(bool onlyReliable = false) = 0;

	virtual const void*& GetRemoteAddress(void) const = 0; // void* => netadr_t => netadr_s
	virtual void** GetMsgHandler(void) const = 0; // void* => INetChannelHandler
	virtual int GetDropNumber(void) const = 0;
	virtual int GetSocket(void) const = 0;
	virtual unsigned int GetChallengeNr(void) const = 0;
	virtual void GetSequenceData(int& nOutSequenceNr, int& nInSequenceNr, int& nOutSequenceNrAck) = 0;
	virtual void SetSequenceData(int nOutSequenceNr, int nInSequenceNr, int nOutSequenceNrAck) = 0;

	virtual void UpdateMessageStats(int msggroup, int bits) = 0;
	virtual bool CanPacket(void) const = 0;
	virtual bool IsOverflowed(void) const = 0;
	virtual bool IsTimedOut(void) const = 0;
	virtual bool HasPendingReliableData(void) = 0;

	virtual void SetFileTransmissionMode(bool bBackgroundMode) = 0;
	virtual void SetCompressionMode(bool bUseCompression) = 0;
	virtual unsigned int RequestFile(const char* filename) = 0;
	virtual float GetTimeSinceLastReceived(void) const = 0; // get time since last received packet in seconds

	virtual void SetMaxBufferSize(bool bReliable, int nBYTEs, bool bVoice = false) = 0;

	virtual bool IsNull() const = 0;
	virtual int GetNumBitsWritten(bool bReliable) = 0;
	virtual void SetInterpolationAmount(float flInterpolationAmount) = 0;
	virtual void SetRemoteFramerate(float flFrameTime, float flFrameTimeStdDeviation) = 0;

	// Max # of payload BYTEs before we must split/fragment the packet
	virtual void SetMaxRoutablePayloadSize(int nSplitSize) = 0;
	virtual int GetMaxRoutablePayloadSize() = 0;

	virtual int GetProtocolVersion() = 0;
};

class INetChannelHandler
{
public:
	virtual ~INetChannelHandler(void)
	{
	};

	virtual void ConnectionStart(INetChannel* chan) = 0; // called first time network channel is established

	virtual void ConnectionClosing(const char* reason) = 0; // network channel is being closed by remote site

	virtual void ConnectionCrashed(const char* reason) = 0; // network error occured

	virtual void PacketStart(int incoming_sequence, int outgoing_acknowledged) = 0; // called each time a new packet arrived

	virtual void PacketEnd(void) = 0; // all messages has been parsed

	virtual void FileRequested(const char* fileName, unsigned int transferID) = 0; // other side request a file for download

	virtual void FileReceived(const char* fileName, unsigned int transferID) = 0; // we received a file

	virtual void FileDenied(const char* fileName, unsigned int transferID) = 0; // a file request was denied by other side

	virtual void FileSent(const char* fileName, unsigned int transferID) = 0; // we sent a file
};

class INetMessage
{
public:
	virtual ~INetMessage()
	{
	};

	// Use these to setup who can hear whose voice.
	// Pass in client indices (which are their ent indices - 1).

	virtual void SetNetChannel(INetChannel* netchan) = 0; // netchannel this message is from/for
	virtual void SetReliable(bool state) = 0;			  // set to true if it's a reliable message

	virtual bool Process(void) = 0; // calles the recently set handler to process this message

	virtual bool ReadFromBuffer(uintptr_t& buffer) = 0; // returns true if parsing was OK
	virtual bool WriteToBuffer(uintptr_t& buffer) = 0; // returns true if writing was OK

	virtual bool IsReliable(void) const = 0; // true, if message needs reliable handling

	virtual int GetType(void) const = 0;		 // returns module specific header tag eg svc_serverinfo
	virtual int GetGroup(void) const = 0;		 // returns net message group of this message
	virtual const char* GetName(void) const = 0; // returns network message name, eg "svc_serverinfo"
	virtual INetChannel* GetNetChannel(void) const = 0;
	virtual const char* ToString(void) const = 0; // returns a human readable string about message content
};


class CNetChan : public INetChannel
{

public: // netchan structurs
	typedef struct dataFragments_s
	{
		void* file;                 // open file handle
		char         filename[260]; // filename
		char* buffer;               // if NULL it's a file
		unsigned int BYTEs;                // size in BYTEs
		unsigned int bits;                 // size in bits
		unsigned int transferID;           // only for files
		bool         isCompressed;         // true if data is bzip compressed
		unsigned int nUncompressedSize;    // full size in BYTEs
		bool         asTCP;                // send as TCP stream
		int          numFragments;         // number of total fragments
		int          ackedFragments;       // number of fragments send & acknowledged
		int          pendingFragments;     // number of fragments send, but not acknowledged yet
	} dataFragments_t;

	struct subChannel_s
	{
		int startFraggment[2];
		int numFragments[2];
		int sendSeqNr;
		int state; // 0 = free, 1 = scheduled to send, 2 = send & waiting, 3 = dirty
		int index; // index in m_SubChannels[]

		void Free()
		{
			state = 0;
			sendSeqNr = -1;
			for (int i = 0; i < 2; i++) {
				numFragments[i] = 0;
				startFraggment[i] = -1;
			}
		}
	};

	// Client's now store the command they sent to the server and the entire results set of
	//  that command.
	typedef struct netframe_s
	{
		// Data received from server
		float          time;        // net_time received/send
		int            size;        // total size in BYTEs
		float          latency;     // raw ping for this packet, not cleaned. set when acknowledged otherwise -1.
		float          avg_latency; // averaged ping for this packet
		bool           valid;       // false if dropped, lost, flushed
		int            choked;      // number of previously chocked packets
		int            dropped;
		float          m_flInterpolationAmount;
		unsigned short msggroups[INetChannelInfo::TOTAL]; // received BYTEs for each message group
	} netframe_t;

	typedef struct
	{
		float       nextcompute;      // Time when we should recompute k/sec data
		float       avgBYTEspersec;   // average BYTEs/sec
		float       avgpacketspersec; // average packets/sec
		float       avgloss;          // average packet loss [0..1]
		float       avgchoke;         // average packet choke [0..1]
		float       avglatency;       // average ping, not cleaned
		float       latency;          // current ping, more accurate also more jittering
		int         totalpackets;     // total processed packets
		int         totalBYTEs;       // total processed BYTEs
		int         currentindex;     // current frame index
		netframe_t  frames[64];       // frame history
		netframe_t* currentframe;     // current frame
	} netflow_t;

public:
	CNetChan() = delete;

	bool m_bProcessingMessages;
	bool m_bShouldDelete;

	// last send outgoing sequence number
	int m_nOutSequenceNr;
	// last received incoming sequnec number
	int m_nInSequenceNr;
	// last received acknowledge outgoing sequnce number
	int m_nOutSequenceNrAck;

	// state of outgoing reliable data (0/1) flip flop used for loss detection
	int m_nOutReliableState;
	// state of incoming reliable data
	int m_nInReliableState;

	int m_nChokedPackets; // number of choked packets
};