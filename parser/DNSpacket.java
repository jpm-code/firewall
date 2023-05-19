import java.nio.ByteBuffer;
import java.util.ArrayList;

public class DNSpacket extends Packet {

	// constructor
	public DNSpacket() {
	
	}


	@Override
	public ArrayList<Byte> getData() {
		// convert ArrayList<Byte> to byte[] and return
		return this.data ;
	}

	@Override
	public int getSize() {
		return this.size ;
	}

	// Implement other abstract methods...
	// This method returns the packet's header as an array of bytes
	@Override
	public byte getHeader() {
		return this.header ;
	}

	@Override
	// This method returns the packet's payload as an array of bytes
	public byte[] getPayload() {
          byte[] myByteArray = new byte[10];
          return myByteArray ;
	}


	public short getType(){ return this.type ; }

	void setHeader( byte header ) { this.header = header; }

	void setFlags( byte flags ) { this.flags = flags; }

	void setType( short type ) { this.type = type; }

	void setSize( int size ) { this.size = size; }

	void setData( byte[] data ) { 
		this.data.clear() ;
		for ( byte b : data ) {
			this.data.add( b ) ;
		}
	}

	void setChecksum( byte checksum ) { this.checksum = checksum; } 

	@Override
	public void serialize( ArrayList<Byte> buf ) {
		
		ByteBuffer buffer = ByteBuffer.allocate(1+1+2 + 4 + data.size() + 1) ;
		buffer.put( header ) ;
		buffer.put( flags ) ;
		buffer.putShort( type ) ;
		buffer.putInt( size ) ;
		
		for ( Byte b : this.data ) {	
		  buffer.put( b.byteValue() ) ;
		} 

		buffer.put( checksum ) ;

		for ( byte b : buffer.array() ) {
			buf.add( b ) ;
		}
	}

}
