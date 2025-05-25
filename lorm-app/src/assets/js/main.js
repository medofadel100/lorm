document.addEventListener('DOMContentLoaded', () => {
    // Only run navigation code if nav elements exist
    const navLinks = document.querySelectorAll('.nav-links a');
    if (navLinks.length > 0) {
        const pages = document.querySelectorAll('.page');
        
        navLinks.forEach(link => {
            link.addEventListener('click', (e) => {
                e.preventDefault();
                const pageId = link.getAttribute('data-page');
                
                navLinks.forEach(l => l.classList.remove('active'));
                link.classList.add('active');
                
                pages.forEach(page => {
                    page.classList.remove('active');
                    if (page.id === pageId) {
                        page.classList.add('active');
                    }
                });
            });
        });
    }

    // Messenger elements
    const messengerElements = {
        connectBtn: document.getElementById('connect-btn'),
        sendBtn: document.getElementById('send-btn'),
        status: document.getElementById('status'),
        messages: document.getElementById('messages'),
        messageInput: document.getElementById('message-input'),
        timestamp: document.getElementById('timestamp')
    };

    if (messengerElements.connectBtn) {
        let isConnected = false;

        // Function to create and append a new message
        const createMessage = (text, type) => {
            const newMessage = document.createElement('div');
            newMessage.className = `message ${type}`;
            
            const messageContent = document.createElement('div');
            messageContent.className = 'message-content';
            messageContent.textContent = text;

            const messageTime = document.createElement('div');
            messageTime.className = 'message-time';
            messageTime.textContent = new Date().toLocaleTimeString();

            newMessage.appendChild(messageContent);
            newMessage.appendChild(messageTime);
            
            messengerElements.messages.appendChild(newMessage);
            messengerElements.messages.scrollTop = messengerElements.messages.scrollHeight;
        };

        // Connect/Disconnect button handler
        messengerElements.connectBtn.addEventListener('click', () => {
            isConnected = !isConnected;
            messengerElements.status.textContent = `Status: ${isConnected ? 'Connected' : 'Disconnected'}`;
            messengerElements.connectBtn.textContent = isConnected ? 'Disconnect LORM Glove' : 'Connect LORM Glove';
            
            // Enable/disable input and send button
            messengerElements.messageInput.disabled = !isConnected;
            messengerElements.sendBtn.disabled = !isConnected;
            
            if (isConnected) {
                createMessage('LORM Glove Connected', 'received');
                messengerElements.messageInput.focus();
            } else {
                createMessage('LORM Glove Disconnected', 'received');
            }
        });

        // Send message function
        const sendMessage = () => {
            if (isConnected && messengerElements.messageInput.value.trim()) {
                createMessage(messengerElements.messageInput.value.trim(), 'sent');
                messengerElements.messageInput.value = '';
                messengerElements.messageInput.focus();
            } else if (!isConnected) {
                alert('Please connect the LORM Glove first');
            }
        };

        // Send button click handler
        if (messengerElements.sendBtn) {
            messengerElements.sendBtn.addEventListener('click', sendMessage);
        }

        // Enter key press handler
        if (messengerElements.messageInput) {
            messengerElements.messageInput.addEventListener('keypress', (e) => {
                if (e.key === 'Enter' && !e.shiftKey) {
                    e.preventDefault();
                    sendMessage();
                }
            });
        }

        // Update timestamp
        if (messengerElements.timestamp) {
            setInterval(() => {
                messengerElements.timestamp.textContent = new Date().toLocaleString();
            }, 1000);
        }

        // Simulate receiving messages (for testing)
        const simulateReceived = () => {
            if (isConnected) {
                const responses = [
                    'Message received',
                    'LORM Glove signal detected',
                    'Processing input...',
                    'Translation complete'
                ];
                const randomResponse = responses[Math.floor(Math.random() * responses.length)];
                createMessage(randomResponse, 'received');
            }
        };

        // Optional: Uncomment to enable message simulation
        // setInterval(simulateReceived, 5000);
    }
});