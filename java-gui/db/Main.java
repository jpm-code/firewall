import com.rethinkdb.RethinkDB;
import com.rethinkdb.gen.exc.ReqlError;
import com.rethinkdb.gen.exc.ReqlQueryLogicError;
import com.rethinkdb.model.MapObject;
import com.rethinkdb.net.Connection;


public class Main {

public static final RethinkDB r = RethinkDB.r;

public static void main(String[] args) {

    

Connection conn = r.connection().hostname("127.0.0.1").port(28015).connect();

r.db("test").tableCreate("us_shows").run(conn);
r.table("us_shows").insert(r.hashMap("name", "Star Trek TNG")).run(conn);

}

}
