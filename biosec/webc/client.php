<!doctype html>
<html>
<head>
  <meta charset="utf-8">
  <title>WebRTC Video Call Demo</title>
  <style>
    video { width: 45%; border: 1px solid #444; margin: 4px; }
    #controls { margin:8px; }
  </style>
</head>
<body>
  <h2>WebRTC Video Call (via PHP signaling)</h2>
  <div id="controls">
    Name: <input id="name" value="user1">
    Room: <input id="room" value="room1">
    <button id="join">Join Room</button>
    <button id="leave">Leave</button>
  </div>

  <div>
    <video id="localVideo" autoplay muted playsinline></video>
    <video id="remoteVideo" autoplay playsinline></video>
  </div>

<script>
const wsUrl = 'ws://'+location.hostname+':8080'; // adjust if needed
let pc = null;
let ws = null;
let localStream = null;
let room = null;
let myId = null;

// STUN servers (public)
const rtcConfig = {
  iceServers: [
    {urls: 'stun:stun.l.google.com:19302'}
  ]
};

document.getElementById('join').onclick = async () => {
  const name = document.getElementById('name').value || 'u';
  room = document.getElementById('room').value || 'room1';
  ws = new WebSocket(wsUrl);
  ws.onopen = () => {
    console.log('WS open');
  };
  ws.onmessage = async (ev) => {
    const msg = JSON.parse(ev.data);
    console.log('WS msg', msg);
    if(msg.type === 'info'){ console.log(msg.text); return; }
    if(msg.type === 'peer-joined'){
      // optionally create offer to new peer
      // wait for peer list workflow — we'll do simple approach: first peer creates offer when second joins
      if(pc && myId){
        // create offer and send to specific peer id
        await createOfferAndSend(msg.id);
      }
    }
    if(msg.type === 'offer'){
      await handleOffer(msg.from, msg.payload);
    }
    if(msg.type === 'answer'){
      await handleAnswer(msg.payload);
    }
    if(msg.type === 'ice'){
      await handleRemoteIce(msg.payload);
    }
    if(msg.type === 'peer-left'){
      console.log('peer left', msg.id);
      // cleanup remote video
      document.getElementById('remoteVideo').srcObject = null;
    }
  };

  // get media
  try {
    localStream = await navigator.mediaDevices.getUserMedia({video:true, audio:true});
    document.getElementById('localVideo').srcObject = localStream;
  } catch(e){
    alert('getUserMedia error: '+e);
    return;
  }

  // create RTCPeerConnection
  pc = new RTCPeerConnection(rtcConfig);
  pc.onicecandidate = (e) => {
    if(e.candidate){
      ws.send(JSON.stringify({type:'ice', room: room, payload: e.candidate}));
    }
  };
  pc.ontrack = (e) => {
    console.log('remote track', e.streams);
    document.getElementById('remoteVideo').srcObject = e.streams[0];
  };

  // add local tracks
  localStream.getTracks().forEach(t => pc.addTrack(t, localStream));

  // join via signaling
  ws.addEventListener('open', () => {
    ws.send(JSON.stringify({type:'join', room: room, name: name}));
  });

  // set myId when ws assigns resource? our signaling server doesn't send id. We'll use pc.identity later if needed.
};

async function createOfferAndSend(toId){
  // create offer destined to peer with resourceId == toId
  const offer = await pc.createOffer();
  await pc.setLocalDescription(offer);
  ws.send(JSON.stringify({type:'offer', room: room, to: toId, payload: offer}));
}

async function handleOffer(fromId, offer){
  // set remote desc and create answer
  await pc.setRemoteDescription(new RTCSessionDescription(offer));
  const answer = await pc.createAnswer();
  await pc.setLocalDescription(answer);
  ws.send(JSON.stringify({type:'answer', room: room, to: fromId, payload: answer}));
}

async function handleAnswer(answer){
  await pc.setRemoteDescription(new RTCSessionDescription(answer));
}

async function handleRemoteIce(candidate){
  try{
    await pc.addIceCandidate(new RTCIceCandidate(candidate));
  }catch(e){ console.warn('addIce error', e); }
}

document.getElementById('leave').onclick = () => {
  if(ws) ws.send(JSON.stringify({type:'leave', room: room}));
  if(pc) pc.close();
  pc = null;
  if(localStream){ localStream.getTracks().forEach(t=>t.stop()); localStream=null; }
};
</script>
</body>
</html>

