import java.util.ArrayList;

public abstract class Packet {

	protected byte header = (byte) 0xAF; // Marks the begin of `Packet`
	protected byte flags = (byte)0x00; // Custom bitmask flags.
	protected short type = (short)0x0000; // Identifies the packet's type - what is it used for.
	protected int size = (int)0x00000000; // Size of `data`.
	protected ArrayList<Byte> data = new ArrayList<>(); // Custom data.
	protected byte checksum = (byte)0x00; // XOR checksum.


	// This method returns the data of the packet as an array of bytes
	public abstract ArrayList<Byte> getData();

	// This method returns the size of the packet
	public abstract int getSize();

	// This method returns the packet's header as an array of bytes
	public abstract byte getHeader();

	// This method returns the packet's payload as an array of bytes
	public abstract byte[] getPayload();

	// Serialize the packet
	public abstract void serialize( ArrayList<Byte> buf ); 

	public static void printHex( ArrayList<Byte> buf ) {
		for (byte b : buf) {
			System.out.print(String.format("%02X ", b & 0xFF));
		}

	}

	public static byte[] toBytes(ArrayList<Byte> byteList){

		// Convert ArrayList to byte[]
		byte[] byteArray = new byte[byteList.size()];
		for (int i = 0; i < byteList.size(); i++) {
			byteArray[i] = byteList.get(i);
		}
            return byteArray ;
	}

}


