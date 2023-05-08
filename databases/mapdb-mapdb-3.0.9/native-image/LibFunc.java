import org.mapdb.*;
import java.util.concurrent.ConcurrentMap;
import org.graalvm.nativeimage.IsolateThread;
import org.graalvm.nativeimage.c.function.CEntryPoint;

public class LibFunc {
	@CEntryPoint(name = "func_call")
	private static void funcCall( IsolateThread thread ) {
		//		DB db = DBMaker.memoryDB().make();
		DB db = DBMaker.fileDB("database.db").make();
		ConcurrentMap map = db.hashMap("map").make();

		String key   = "google.com" ;
		String value = "" ;
		/*
		   System.out.println( "Put - Key: " + key + "; Value: " + value );

		   map.put( key, value );
		   */

		key   = "google.com" ;

		value = (String) map.get( key );

		System.out.println( "Got - Key: " + key + "; Value: " + value );

		key   = "bing.com" ;

		value = (String) map.get( key );

		System.out.println( "Got - Key: " + key + "; Value: " + value );

		db.close();
	}
}

