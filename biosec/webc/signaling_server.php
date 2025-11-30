<?php
require __DIR__ . '/vendor/autoload.php';

use Ratchet\MessageComponentInterface;
use Ratchet\ConnectionInterface;

class Signaler implements MessageComponentInterface {
    protected $clients;
    // map resourceId -> username (optional)
    protected $meta;

    public function __construct(){
        $this->clients = new \SplObjectStorage;
        $this->meta = [];
        echo "Signaling server started\n";
    }

    public function onOpen(ConnectionInterface $conn){
        $this->clients->attach($conn);
        $this->meta[$conn->resourceId] = ['room'=>null, 'name'=>null];
        $conn->send(json_encode(['type'=>'info','text'=>'Welcome to signaling server']));
    }

    public function onMessage(ConnectionInterface $from, $msg){
        $data = json_decode($msg, true);
        if(!$data) return;

        // message format: { type: "join"/"offer"/"answer"/"ice"/"leave", room: "room1", to: optional, payload: {} }
        $type = $data['type'] ?? '';

        if($type === 'join'){
            $room = $data['room'] ?? null;
            $name = $data['name'] ?? ('u'.$from->resourceId);
            $this->meta[$from->resourceId]['room'] = $room;
            $this->meta[$from->resourceId]['name'] = $name;
            // notify others in room
            foreach($this->clients as $client){
                if($client !== $from && $this->meta[$client->resourceId]['room'] === $room){
                    $client->send(json_encode(['type'=>'peer-joined','id'=>$from->resourceId,'name'=>$name]));
                }
            }
            return;
        }

        if(in_array($type, ['offer','answer','ice','leave'])){
            $room = $data['room'] ?? $this->meta[$from->resourceId]['room'];
            $to = $data['to'] ?? null; // optionally send to single peer (resourceId)
            $payload = $data['payload'] ?? null;

            // If 'to' provided -> direct
            if($to){
                foreach($this->clients as $client){
                    if($client->resourceId == $to){
                        $client->send(json_encode(['type'=>$type,'from'=>$from->resourceId,'payload'=>$payload]));
                        break;
                    }
                }
            } else {
                // broadcast to room (except sender)
                foreach($this->clients as $client){
                    if($client !== $from && $this->meta[$client->resourceId]['room'] === $room){
                        $client->send(json_encode(['type'=>$type,'from'=>$from->resourceId,'payload'=>$payload]));
                    }
                }
            }
        }
    }

    public function onClose(ConnectionInterface $conn){
        $room = $this->meta[$conn->resourceId]['room'] ?? null;
        $name = $this->meta[$conn->resourceId]['name'] ?? null;
        unset($this->meta[$conn->resourceId]);

        // notify others
        foreach($this->clients as $client){
            if($client !== $conn && $this->meta[$client->resourceId]['room'] === $room){
                $client->send(json_encode(['type'=>'peer-left','id'=>$conn->resourceId,'name'=>$name]));
            }
        }

        $this->clients->detach($conn);
    }

    public function onError(ConnectionInterface $conn, \Exception $e){
        echo "Error: {$e->getMessage()}\n";
        $conn->close();
    }
}

// Run server
$port = 8080;
$server = Ratchet\Server\IoServer::factory(
    new Ratchet\Http\HttpServer(
        new Ratchet\WebSocket\WsServer(
            new Signaler()
        )
    ),
    $port
);

echo "Running signaling server on :{$port}\n";
$server->run();

