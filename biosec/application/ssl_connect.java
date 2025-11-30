import javax.net.ssl.*;
import java.io.*;
import java.security.KeyStore;

public class SSLConnection {

    public static HttpsURLConnection getSecureConnection(String urlStr) throws Exception {

        // Load truststore ใส่ cert.crt ไว้ในนี้
        char[] pass = "password".toCharArray();
        KeyStore ts = KeyStore.getInstance("JKS");
        ts.load(new FileInputStream("mytruststore.jks"), pass);

        TrustManagerFactory tmf = TrustManagerFactory.getInstance("SunX509");
        tmf.init(ts);

        SSLContext ssl = SSLContext.getInstance("TLS");
        ssl.init(null, tmf.getTrustManagers(), null);

        HttpsURLConnection.setDefaultSSLSocketFactory(ssl.getSocketFactory());
        HttpsURLConnection.setDefaultHostnameVerifier((h, s) -> true);

        return (HttpsURLConnection) new java.net.URL(urlStr).openConnection();
    }
}

