package javax.virtualsense.network.protocols.minpath;

import javax.virtualsense.network.BroadcastPacket;

public class InterestMsg extends BroadcastPacket {
	public short epoch;
	public short hops;
	public short nodeID;  //the node id
}