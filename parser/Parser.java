
public class Parser {
    private int step = 0;
    private int bytesRead = 0;

    public int getBytesRead() {
      return bytesRead ;
    }
    
    public boolean parse(byte[] data, int len, DNSpacket packet) {
	this.bytesRead = 0;
        for (int i = 0; i < len; i++) {
            byte b = data[i];
            bytesRead++;

            switch (step) {
                case 0: // Header
                    if (b != (byte) 0xAF) {
                        step = 0;
                        continue;
                    }
                    packet.setHeader(b);
                    step++;
                    break;

                case 1: // Flags
                    packet.setFlags(b);
                    step++;
                    break;

                case 2: // Type (2 bytes!)
                    packet.setType((short) ((b & 0xFF) << 8));
                    step++;
                    break;

                case 3:
                    packet.setType((short) (packet.getType() | (b & 0xFF)));
                    step++;
                    break;

                case 4: // Size (4 bytes!)
                    packet.setSize((b & 0xFF) << 24);
                    step++;
                    break;

                case 5:
                    packet.setSize(packet.getSize() | ((b & 0xFF) << 16));
                    step++;
                    break;

                case 6:
                    packet.setSize(packet.getSize() | ((b & 0xFF) << 8));
                    step++;
                    break;

                case 7:
                    packet.setSize(packet.getSize() | (b & 0xFF));
                    packet.getData().clear();
                    if (packet.getSize() > 0) {
                        packet.getData().ensureCapacity(packet.getSize());
                    } else {
                        step++; // Skip data step
                    }
                    step++;
                    break;

                case 8: // Data
                    packet.getData().add(b);
                    if (packet.getData().size() == packet.getSize())
                        step++;
                    break;

                case 9: // Checksum
                    packet.setChecksum(b);
                    step = 0;
                    return true;
            }
        }
	
        return false;
    }
}

