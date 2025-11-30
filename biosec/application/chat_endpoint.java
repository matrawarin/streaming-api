import javax.websocket.*;
import javax.websocket.server.ServerEndpoint;
import java.util.concurrent.ConcurrentHashMap;
import javax.json.*;

@ServerEndpoint("/chat")
public class ChatEndpoint {
    private static ConcurrentHashMap<Session, String> names = new ConcurrentHashMap<>();

    @OnOpen
    public void onOpen(Session session){ session.getAsyncRemote().sendText("{\"type\":\"system\",\"text\":\"Send register JSON\"}"); }

    @OnMessage
    public void onMessage(Session session, String msg) {
        JsonObject obj = Json.createReader(new java.io.StringReader(msg)).readObject();
        String type = obj.getString("type");
        if("register".equals(type)){
            names.put(session, obj.getString("name"));
            broadcast("{\"type\":\"system\",\"text\":\""+obj.getString("name")+" joined\"}");
            return;
        }
        if("message".equals(type)){
            String to = obj.containsKey("to") ? obj.getString("to") : null;
            if(to!=null && !to.isEmpty()){
                for(Session s : names.keySet()){
                    if(to.equals(names.get(s))){
                        s.getAsyncRemote().sendText(Json.createObjectBuilder()
                            .add("type","private").add("from", names.get(session)).add("text", obj.getString("text")).build().toString());
                        return;
                    }
                }
                session.getAsyncRemote().sendText("{\"type\":\"error\",\"text\":\"User not found\"}");
            } else {
                broadcast(Json.createObjectBuilder().add("type","message").add("from",names.get(session)).add("text",obj.getString("text")).build().toString());
            }
        }
    }

    @OnClose
    public void onClose(Session session){ String name = names.remove(session); broadcast("{\"type\":\"system\",\"text\":\""+name+" left\"}"); }

    private void broadcast(String text){
        names.keySet().forEach(s -> s.getAsyncRemote().sendText(text));
    }
}

