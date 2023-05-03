import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;

import com.rethinkdb.RethinkDB;
import com.rethinkdb.gen.exc.ReqlError;
import com.rethinkdb.gen.exc.ReqlQueryLogicError;
import com.rethinkdb.model.MapObject;
import com.rethinkdb.net.Connection;


public class Controller {

    public static final RethinkDB r = RethinkDB.r;

    public static void main(String[] args) throws IOException {

	Connection conn = r.connection().hostname("172.20.10.4").port(28015).connect();

	if(conn == null) {
		System.exit(-1);
	}
	else {
		System.out.println("Connection made\n");
	}

	r.db("test").tableCreate("messages").run(conn);

        // The command to run, you can replace this with your executable or script
        //String[] command = {"cmd.exe", "/c", "echo", "Hello, World!"};
	String[] command = { "./slave.exe" };

        // Start the process using ProcessBuilder
        ProcessBuilder processBuilder = new ProcessBuilder(command);
        Process process = processBuilder.start();

        // Get the input, output, and error streams
        OutputStream stdin = process.getOutputStream();
        InputStreamReader stdoutReader = new InputStreamReader(process.getInputStream());
        InputStreamReader stderrReader = new InputStreamReader(process.getErrorStream());

        // Send data to the process through its stdin
        PrintWriter stdinWriter = new PrintWriter(new OutputStreamWriter(stdin), true);
        stdinWriter.println("MSG_CIN: " + Math.random() );

        // Receive data from the process through its stdout
        BufferedReader stdoutBufferedReader = new BufferedReader(stdoutReader);
        String line;
        System.out.println("Standard Output (exe -> Java):");
	
        while ((line = stdoutBufferedReader.readLine()) != null) {
            System.out.println(line);
	    String[] messageParts = line.split("\r?\n|\r");
            r.table("messages").insert( r.hashMap("message", messageParts[0]).with("value", messageParts[1]) );						      
        }

        // Receive data from the process through its stderr
        BufferedReader stderrBufferedReader = new BufferedReader(stderrReader);
        System.out.println("Standard Error (exe -> Java):");
        while ((line = stderrBufferedReader.readLine()) != null) {
            System.out.println(line);
        }

        // Clean up the resources
        stdoutBufferedReader.close();
        stderrBufferedReader.close();
        stdinWriter.close();
    }
}

