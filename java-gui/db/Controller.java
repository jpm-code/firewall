import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;

import com.rethinkdb.RethinkDB;
import com.rethinkdb.gen.exc.ReqlDriverError;
import com.rethinkdb.gen.exc.ReqlOpFailedError;
import com.rethinkdb.gen.exc.ReqlError;
import com.rethinkdb.gen.exc.ReqlQueryLogicError;
import com.rethinkdb.model.MapObject;
import com.rethinkdb.net.Connection;
import com.rethinkdb.net.Result;

import java.util.LinkedHashMap;
import java.util.Map;
import java.util.Random;

public class Controller {

	private static final boolean DEBUG_MODE = true;

	// RethinkDB declarations
	public static final String DATABASE = "test";
	public static final String TABLE    = "messages";
	public static final RethinkDB r = RethinkDB.r;

	public static int randNum() {
		Random random = new Random();
		int randomNumber = random.nextInt(100) + 1;
		return randomNumber;
	}

	private static String getComputerName()
	{
		Map<String, String> env = System.getenv();
		if (env.containsKey("COMPUTERNAME")) {
			return env.get("COMPUTERNAME");
		}
		else if (env.containsKey("HOSTNAME")) {
			return env.get("HOSTNAME");
		}
		else {
			return "Unknown Computer";
		}
	}

	public static void main(String[] args) throws IOException {

		String machine = getComputerName()                + " ; " +
			         System.getProperty("os.name")    + " ; " +
				 System.getProperty("os.version") + " ; " +
				 System.getProperty("os.arch") ;


		// Attempt to connect to the database server
		Connection conn = null;
		try {
			conn = r.connection().hostname("165.22.118.127").port(28015).connect();
		}
		catch (ReqlDriverError ex) {
			System.out.println("Error: No connection, quitting.");
			System.exit(-1);
		}

		//Check the table exists
		try {
			r.db(DATABASE).tableCreate(TABLE).run(conn);
			if(DEBUG_MODE == true) {
				System.out.println("Info: Created table, continuing..");
			}
		}
		catch (ReqlOpFailedError ex) {
			// Table already exists exception thrown
		}

		// The command to run, you can replace this with your executable or script
		//String[] command = {"cmd.exe", "/c", "echo", "Hello, World!"};
		String[] command = { "./slave.exe" };

		// Start the process using ProcessBuilder
		// Need to handle:
		// IOException: This exception is thrown if an I/O error occurs when starting the process. 
		// SecurityException: is thrown if the security manager denies permission to start the process.
		ProcessBuilder processBuilder = new ProcessBuilder(command);
		Process process = processBuilder.start();

		// Get the input, output, and error streams
		OutputStream stdin = process.getOutputStream();
		InputStreamReader stdoutReader = new InputStreamReader(process.getInputStream());
		InputStreamReader stderrReader = new InputStreamReader(process.getErrorStream());

		// Send data to the process through its stdin
		PrintWriter stdinWriter = new PrintWriter(new OutputStreamWriter(stdin), true);
		stdinWriter.println("MSG_CIN: " + randNum() );

		// Receive data from the process through its stdout
		BufferedReader stdoutBufferedReader = new BufferedReader(stdoutReader);
		String line;
		//System.out.println("Standard Output (exe -> Java):");

		while ((line = stdoutBufferedReader.readLine()) != null) {
			System.out.println(line);

			String[] messageParts = line.split("\\s+");

			// Get the ReQL query result of insertion
			// RethinkDB write operations will only throw exceptions if errors occur before any writes. 
			// To properly handle errors with this term, code must both handle exceptions and check the errors return value.
			// https://rethinkdb.com/api/java/insert
			// Exception thrown?
			try {
				//Result result = r.table(TABLE).insert(r.hashMap("name", messageParts[1] )).run(conn) ; 
				Result result = r.table(TABLE).insert(r.hashMap( "IP",     "127.0.0.1" )
						                          .with( "domain", "google.com" )
									  .with( "action", "allowed" )
						                          .with( "machine", machine) 
							             ).run(conn) ;
				//r.table("messages").insert(r.hashMap("message", messageParts[0]).with("value", messageParts[1]) );
				LinkedHashMap resultHashMap = (LinkedHashMap)result.first() ;
				long errors = (long)resultHashMap.get("errors") ;
				if (errors != 0) {
					System.out.println("Error: Errors during insertion.");
					System.exit(-1) ;
				}
			} 
			catch(Exception ex) {
				System.out.println("Error: Exception during insertion - " + ex.toString() );
				System.exit(-1) ;
			}

		}

		// Receive data from the process through its stderr
		//      BufferedReader stderrBufferedReader = new BufferedReader(stderrReader);
		//      System.out.println("Standard Error (exe -> Java):");
		//      while ((line = stderrBufferedReader.readLine()) != null) {
		//          System.out.println(line);
		//    }


		// Clean up the resources
		stdoutBufferedReader.close();
		//        stderrBufferedReader.close();
		stdinWriter.close();

	}
}


