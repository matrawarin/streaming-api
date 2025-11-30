import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.web.WebView;
import javafx.stage.Stage;
import java.nio.file.*;

public class WebRtcApp extends Application {
    @Override
    public void start(Stage stage) throws Exception {
        WebView view = new WebView();
        // load local client.html (ensure path correct)
        String htmlPath = Paths.get("client.html").toUri().toString();
        view.getEngine().load(htmlPath);
        Scene scene = new Scene(view, 1000, 700);
        stage.setTitle("Video Call (WebRTC) - JavaFX Wrapper");
        stage.setScene(scene);
        stage.show();
    }

    public static void main(String[] args) {
        launch(args);
    }
}

