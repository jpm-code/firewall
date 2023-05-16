import java.util.Map;
import org.graalvm.nativeimage.IsolateThread;
import org.graalvm.nativeimage.c.function.CEntryPoint;
import org.graalvm.nativeimage.c.type.CCharPointer;
import org.graalvm.nativeimage.c.type.CTypeConversion;

public class LibEnvMap {
	//NOTE: this class has no main() method

	public static int res = 0 ;

	@CEntryPoint(name = "add")
	private static int add(IsolateThread thread, int a, int b) {
		res = a + b + res;
		return res;
	}

	@CEntryPoint(name = "multadd")
	private static int multadd(IsolateThread thread, int a, int b) {
		int ans = a * b + res ;
		return ans ;
	}

}


