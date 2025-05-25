<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>LORM Messenger - Client 2</title>
    <link rel="stylesheet" href="../assets/css/style.css">
    <script type="module" src="https://www.gstatic.com/firebasejs/9.6.0/firebase-app.js"></script>
    <script type="module" src="https://www.gstatic.com/firebasejs/9.6.0/firebase-database.js"></script>
    <script type="module" src="../config/firebase.js"></script>
</head>
<body>

<div class="messenger-container">
    <div class="messages-panel">
        <div class="message-box">
            <div class="message-header">
                <h3>LORM Messenger - Client 2</h3>
                <div class="status-indicator">
                    <span id="status">Disconnected</span>
                    <span id="firebase-status"></span>
                </div>
            </div>
            <div id="messages" class="messages-list">
                <div class="loading-message">Welcome to LORM Messenger - Client 2</div>
            </div>
            <div class="message-input-container">
                <input type="text" 
                       id="message-input" 
                       placeholder="Type a message as Client 2..." 
                       disabled>
                <button id="send-btn" disabled>
                    <svg viewBox="0 0 24 24" width="24" height="24">
                        <path fill="currentColor" d="M2.01 21L23 12 2.01 3 2 10l15 2-15 2z"></path>
                    </svg>
                </button>
            </div>
        </div>
    </div>
    <div class="control-panel">
        <button id="connect-btn">Connect Client 2</button>
        <div class="device-info">
            <div id="timestamp"></div>
            <div id="connection-info"></div>
        </div>
    </div>
</div>

<script type="module">
    import { database } from '/lorm/lorm-app/src/config/firebase.js';
    import { ref, push, onValue, onDisconnect } from "https://www.gstatic.com/firebasejs/9.6.0/firebase-database.js";
    
    const elements = {
        messages: document.getElementById('messages'),
        messageInput: document.getElementById('message-input'),
        sendBtn: document.getElementById('send-btn'),
        connectBtn: document.getElementById('connect-btn'),
        status: document.getElementById('status'),
        firebaseStatus: document.getElementById('firebase-status'),
        timestamp: document.getElementById('timestamp'),
        connectionInfo: document.getElementById('connection-info')
    };

    let isConnected = false;
    let processedMessages = new Set(); // Track processed messages
    const txRef = ref(database, 'rx'); // Client 2 sends to rx
    const rxRef = ref(database, 'tx'); // Client 2 receives from tx
    const connectedRef = ref(database, '.info/connected');

    function updateMessages(data, type) {
        if (!data || !elements.messages) return;
        
        const fragment = document.createDocumentFragment();
        let hasNewMessages = false;
        
        Object.entries(data)
            .sort(([,a], [,b]) => a.timestamp - b.timestamp)
            .forEach(([key, msg]) => {
                const messageId = `${type}-${key}`;
                if (processedMessages.has(messageId)) return;
                
                hasNewMessages = true;
                processedMessages.add(messageId);

                const messageDiv = document.createElement('div');
                messageDiv.id = `message-${messageId}`;
                const isFromClient2 = msg.sender === 'Client 2';
                messageDiv.className = `message ${isFromClient2 ? 'sent' : 'received'}`;
                
                messageDiv.innerHTML = `
                    <div class="message-bubble">
                        <div class="message-sender">${msg.sender || 'Client 1'}</div>
                        <div class="message-content">${msg.text}</div>
                        <div class="message-time">${new Date(msg.timestamp).toLocaleTimeString()}</div>
                    </div>
                `;
                
                fragment.appendChild(messageDiv);
            });

        if (hasNewMessages) {
            elements.messages.appendChild(fragment);
            elements.messages.scrollTop = elements.messages.scrollHeight;
        }
    }

    async function sendMessage(text) {
        if (!text.trim() || !isConnected) return;
        
        try {
            const timestamp = Date.now();
            await push(txRef, {
                text: text.trim(),
                timestamp: timestamp,
                type: 'rx',
                sender: 'Client 2',
                clientId: '2'
            });
            elements.timestamp.textContent = `Last sent: ${new Date(timestamp).toLocaleString()}`;
            elements.messageInput.value = '';
            elements.messageInput.focus();
        } catch (error) {
            console.error('Error sending message:', error);
            elements.connectionInfo.textContent = `Error: ${error.message}`;
        }
    }

    // Firebase connection status
    onValue(connectedRef, (snap) => {
        const connected = snap.val();
        elements.firebaseStatus.textContent = connected ? '🟢' : '🔴';
        elements.firebaseStatus.title = `Firebase: ${connected ? 'Connected' : 'Disconnected'}`;
        
        if (connected) {
            onDisconnect(txRef).update({
                status: 'offline',
                lastSeen: Date.now()
            });
        }
    });

    // Event Listeners
    elements.connectBtn.addEventListener('click', () => {
        isConnected = !isConnected;
        elements.connectBtn.textContent = isConnected ? 'Disconnect Client 2' : 'Connect Client 2';
        elements.status.textContent = isConnected ? 'Connected' : 'Disconnected';
        elements.status.className = isConnected ? 'status-connected' : 'status-disconnected';
        elements.messageInput.disabled = !isConnected;
        elements.sendBtn.disabled = !isConnected;
        
        if (isConnected) {
            sendMessage('Client 2 Connected');
            elements.messageInput.focus();
        } else {
            sendMessage('Client 2 Disconnected');
        }
    });

    elements.sendBtn.addEventListener('click', () => {
        sendMessage(elements.messageInput.value);
    });

    elements.messageInput.addEventListener('keypress', (e) => {
        if (e.key === 'Enter' && !e.shiftKey) {
            e.preventDefault();
            sendMessage(elements.messageInput.value);
        }
    });

    // Firebase Listeners
    onValue(rxRef, (snapshot) => {
        const data = snapshot.val();
        if (data) updateMessages(data, 'received');
    });

    onValue(txRef, (snapshot) => {
        const data = snapshot.val();
        if (data) updateMessages(data, 'sent');
    });

    // Update timestamp
    setInterval(() => {
        if (elements.timestamp) {
            elements.timestamp.textContent = new Date().toLocaleString();
        }
    }, 1000);

    // Clear processed messages cache periodically
    setInterval(() => {
        const oneHourAgo = Date.now() - (60 * 60 * 1000);
        processedMessages = new Set(
            Array.from(processedMessages).filter(id => {
                const el = document.getElementById(`message-${id}`);
                return el && el.dataset.timestamp > oneHourAgo;
            })
        );
    }, 60 * 60 * 1000); // Every hour
</script>

<style>
/* Container Layout */
.messenger-container {
    display: flex;
    gap: 20px;
    padding: 20px;
    max-width: 1200px;
    margin: 0 auto;
    height: calc(100vh - 100px);
}

.messages-panel {
    flex: 1;
    min-width: 0;
}

/* Message Box */
.message-box {
    display: flex;
    flex-direction: column;
    height: 100%;
    border-radius: 12px;
    background: #fff;
    box-shadow: 0 2px 10px rgba(0,0,0,0.1);
}

/* Header Styles - Client 2 Specific */
.message-header {
    display: flex;
    align-items: center;
    justify-content: space-between;
    padding: 16px;
    background: #1a237e;
    color: white;
    border-radius: 12px 12px 0 0;
}

.message-header h3 {
    margin: 0;
    font-size: 1.2em;
    font-weight: 600;
}

/* Messages Container */
.messages-list {
    flex: 1;
    overflow-y: auto;
    padding: 16px;
    background: #e5ddd5;
    scroll-behavior: smooth;
}

/* Message Bubbles */
.message {
    max-width: 70%;
    margin: 8px 0;
    position: relative;
    clear: both;
    animation: messageIn 0.3s ease-out;
}

.message-bubble {
    padding: 8px 12px;
    border-radius: 12px;
    position: relative;
    box-shadow: 0 1px 2px rgba(0,0,0,0.1);
}

/* Received Messages */
.message.received {
    float: left;
}

.message.received .message-bubble {
    background: #fff;
    border-bottom-left-radius: 4px;
}

/* Sent Messages */
.message.sent {
    float: right;
}

.message.sent .message-bubble {
    background: #dcf8c6;
    border-bottom-right-radius: 4px;
}

/* Message Components */
.message-sender {
    font-size: 0.75em;
    color: #1a237e;
    margin-bottom: 4px;
    font-weight: 600;
}

.message.sent .message-sender {
    text-align: right;
    color: #1a237e;
}

.message-content {
    margin-bottom: 4px;
    word-wrap: break-word;
    line-height: 1.4;
    font-size: 0.95em;
}

.message-time {
    font-size: 0.7em;
    color: #888;
    text-align: right;
    margin-top: 2px;
}

/* Input Area */
.message-input-container {
    display: flex;
    align-items: center;
    gap: 8px;
    padding: 12px;
    background: #f0f0f0;
    border-radius: 0 0 12px 12px;
    border-top: 1px solid rgba(0,0,0,0.05);
}

#message-input {
    flex: 1;
    padding: 12px;
    border: none;
    border-radius: 24px;
    background: #fff;
    font-size: 15px;
    transition: box-shadow 0.2s;
}

#message-input:focus {
    outline: none;
    box-shadow: 0 0 0 2px rgba(26,35,126,0.2);
}

#message-input:disabled {
    background: #f5f5f5;
    cursor: not-allowed;
}

/* Send Button - Client 2 Specific */
#send-btn {
    width: 40px;
    height: 40px;
    border: none;
    border-radius: 50%;
    background: #1a237e;
    color: white;
    padding: 8px;
    cursor: pointer;
    transition: background 0.2s, transform 0.1s;
}

#send-btn:hover:not(:disabled) {
    background: #283593;
    transform: scale(1.05);
}

#send-btn:disabled {
    background: #ccc;
    cursor: not-allowed;
}

/* Control Panel */
.control-panel {
    width: 280px;
    padding: 16px;
    background: #fff;
    border-radius: 12px;
    box-shadow: 0 2px 10px rgba(0,0,0,0.1);
}

#connect-btn {
    width: 100%;
    padding: 12px;
    background: #1a237e;
    color: white;
    border: none;
    border-radius: 8px;
    font-size: 1em;
    cursor: pointer;
    transition: background 0.2s;
}

#connect-btn:hover {
    background: #283593;
}

.device-info {
    margin-top: 16px;
    padding: 12px;
    background: #f5f5f5;
    border-radius: 8px;
    font-size: 0.9em;
}

/* Status Indicators */
.status-connected {
    color: #4caf50;
    font-weight: bold;
}

.status-disconnected {
    color: #f44336;
    font-weight: bold;
}

.status-indicator {
    display: flex;
    align-items: center;
    gap: 8px;
}

#firebase-status {
    font-size: 12px;
}

/* Loading Message */
.loading-message {
    text-align: center;
    color: #666;
    padding: 20px;
    font-style: italic;
}

/* Message Animations */
@keyframes messageIn {
    from {
        opacity: 0;
        transform: translateY(20px);
    }
    to {
        opacity: 1;
        transform: translateY(0);
    }
}

/* Clearfix for floating messages */
.messages-list::after {
    content: '';
    display: table;
    clear: both;
}

/* Scrollbar Styling */
.messages-list::-webkit-scrollbar {
    width: 6px;
}

.messages-list::-webkit-scrollbar-track {
    background: rgba(0,0,0,0.1);
    border-radius: 3px;
}

.messages-list::-webkit-scrollbar-thumb {
    background: rgba(0,0,0,0.2);
    border-radius: 3px;
}

.messages-list::-webkit-scrollbar-thumb:hover {
    background: rgba(0,0,0,0.3);
}
</style>

<?php include_once '../includes/footer.php'; ?>