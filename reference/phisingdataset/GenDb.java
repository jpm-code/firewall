
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;

import org.mapdb.DB;
import org.mapdb.DBMaker;
import org.mapdb.HTreeMap;
import org.mapdb.Serializer;

import java.nio.charset.StandardCharsets;

public class GenDb {

	static int DEBUG = 0;

	public static void main(String[] args) {

		// Check if the required arguments are provided
		if (args.length < 2) {
			System.err.println("Usage: ./program <filename> <dbname>") ;
			System.err.println("e.g. : ./program  out.txt    mydb.db") ;
			System.exit(-1) ;
		}

		// Get the filename and dbname from command-line arguments
		String filename = args[0] ;
		String dbname   = args[1] ;

		// Create or open the MapDB database
		DB db = DBMaker.fileDB( args[1] ).make(); //mydb.db

		// Create or get the map within the database
		HTreeMap<String, Integer> map = db.hashMap("myMap")
			.keySerializer(Serializer.STRING)
			.valueSerializer(Serializer.INTEGER)
			.createOrOpen();

		// First, calculate the total number of lines in the file
		long totalLines = 0;
		try {
			totalLines = Files.lines(Paths.get(filename)).count();
		} catch (IOException e) {
			e.printStackTrace();
			System.exit(-1);
		}

		// Then, read and process the file, displaying the progress
		try (BufferedReader br = new BufferedReader(new FileReader(filename))) {
			String line;
			long linesProcessed = 0;
			while ((line = br.readLine()) != null) {
				linesProcessed++;
				double progress = (double) linesProcessed / totalLines * 100;
				System.out.print("\rProcessing file... " + String.format("%.2f", progress) + "% completed");

				String[] parts = line.split(",");
				if (parts.length == 2) {
					String website = parts[0].trim();
					try {
						int value = Integer.parseInt(parts[1].trim());
						if( value == 0 || value == 1) {
							if(DEBUG == 1) System.out.println("Putting: " + website + " " + value );
							map.put(website, value);
						}
						else {
							System.out.println("Error - invalid number: " + value);
							System.exit(-1);

						}

					} catch (NumberFormatException e) {
						// Handle the exception
						System.err.println("Invalid number format: " + e.getMessage());
						System.exit(-1);
					}
				}
			}
		} catch (IOException e) {
			e.printStackTrace();
		}

		// Commit and close the database
		db.commit();
		db.close();
	}
}


