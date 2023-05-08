import org.mapdb._
import java.util.concurrent.ConcurrentMap
//import org.graalvm.nativeimage.IsolateThread
//import org.graalvm.nativeimage.c.function.CEntryPoint

object LibFunc {
  
  def funcCall(): Unit = {
    // val db: DB = DBMaker.memoryDB().make()
    val db: DB = DBMaker.fileDB("database.db").make()
    val map: ConcurrentMap[String, String] = db.hashMap("map").make().asInstanceOf[ConcurrentMap[String, String]]

    val key = "google.com"
    var value = ""

    value = map.get(key)
    println(s"Got - Key: $key; Value: $value")

    val key2 = "bing.com"
    value = map.get(key2)
    println(s"Got - Key: $key2; Value: $value")

    db.close()
  }
}

