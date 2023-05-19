import java.util.ArrayList ;


class Main {

	public static void main(String[] args) {

		ArrayList<Byte> buf = new ArrayList<>();

		DNSpacket pkt1 = new DNSpacket();
		pkt1.setHeader( (byte) 0xAF ) ;
		pkt1.setFlags( (byte)0x00 ) ;
		pkt1.setType( (short)0x0001 ) ;
		pkt1.setSize( 3 ) ;
		byte[] data = {(byte)0xFF, (byte)0xFF, (byte) 0xff};
		pkt1.setData( data ) ;
		pkt1.checksum = (byte)0xFF;
		pkt1.serialize(buf);
		Packet.printHex( buf ) ;


		// Parsing.

		Parser parser = new Parser();
		DNSpacket packet = new DNSpacket();
		byte[] p = Packet.toBytes( buf ) ;
		int plen = buf.size();
		while (plen > 0) {
			
			if (parser.parse(p, plen, packet)) {
				// At this point the `packet` is complete.
				System.out.printf( "INFO new packet! type=%d; size=%d\n", packet.getType(), packet.getSize() ) ;
			}
			System.out.println( parser.getBytesRead() ) ;
			//p = p + bytesRead;
			plen = plen - parser.getBytesRead() ;
		}


	}

}

