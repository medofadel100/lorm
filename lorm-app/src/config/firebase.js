import { initializeApp } from "https://www.gstatic.com/firebasejs/9.6.0/firebase-app.js";
import { getDatabase } from "https://www.gstatic.com/firebasejs/9.6.0/firebase-database.js";

const firebaseConfig = {
    apiKey: "AIzaSyD4LqQrYk-6PtfbGkgZodgqQvKPhKp3Trc",
    authDomain: "lorm-zhite.firebaseapp.com",
    databaseURL: "https://lorm-zhite-default-rtdb.firebaseio.com",
    projectId: "lorm-zhite",
    storageBucket: "lorm-zhite.firebasestorage.app",
    messagingSenderId: "869050501746",
    appId: "1:869050501746:web:8a554a47d9dcbd0c3990eb",
    measurementId: "G-K73QDSF2E4"
};

const app = initializeApp(firebaseConfig);
const database = getDatabase(app);

export { database };