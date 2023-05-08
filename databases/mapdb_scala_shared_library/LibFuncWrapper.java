//import org.graalvm.nativeimage.IsolateThread;
//import org.graalvm.nativeimage.c.function.CEntryPoint;


/*public class LibFuncWrapper {
  @CEntryPoint(name = "func_call")
  private static void funcCall(IsolateThread thread) {
	//LibFunc$.MODULE$.funcCall(thread);
	LibFunc.funcCall();
  }
}
*/

import org.mapdb.*;
import java.util.concurrent.ConcurrentMap;
import org.graalvm.nativeimage.IsolateThread;
import org.graalvm.nativeimage.c.function.CEntryPoint;

public class LibFuncWrapper {
        @CEntryPoint(name = "func_call")
        private static void funcCall( IsolateThread thread ) {

                LibFunc.funcCall();
              
	}

}
