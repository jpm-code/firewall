import org.mapdb.* ;
import java.util.concurrent.ConcurrentMap ;
import org.graalvm.nativeimage.IsolateThread ;
import org.graalvm.nativeimage.c.function.CEntryPoint ;
import org.graalvm.nativeimage.c.type.CCharPointer ;
import org.graalvm.nativeimage.c.type.CTypeConversion ;

public class LibNetworkProcessorWrapper {
       @CEntryPoint(name = "parse_dns_payload")
       private static int parseDnsPayload( IsolateThread thread, CCharPointer buffer ) {
//	public static void main(String[] args) {
		System.out.println( "parse_dns_payload()" );
		String buffer_java = CTypeConversion.toJavaString( buffer ) ;
		System.out.println( "parse_dns_payload() passed: " + buffer_java );
                LibFunc.funcCall( buffer_java ) ;
		if( buffer_java.contains( "bing") ) {
		  System.out.println( "Found (Java): bing" );
		  return -1 ; 
		} else {
		  return 0 ;
		}
	}
}

