importScripts("https://www.gstatic.com/firebasejs/7.16.1/firebase-app.js");
importScripts(
    "https://www.gstatic.com/firebasejs/7.16.1/firebase-messaging.js",
);
// For an optimal experience using Cloud Messaging, also add the Firebase SDK for Analytics.
importScripts(
    "https://www.gstatic.com/firebasejs/7.16.1/firebase-analytics.js",
);

// Initialize the Firebase app in the service worker by passing in the
// messagingSenderId.
var firebaseConfig = {
    apiKey: "AIzaSyARWZzmjIfRLHXU-C5xXONkqSArJCBmDxQ",
    authDomain: "esp32-92e64.firebaseapp.com",
    databaseURL: "https://esp32-92e64-default-rtdb.asia-southeast1.firebasedatabase.app",
    projectId: "esp32-92e64",
    storageBucket: "esp32-92e64.appspot.com",
    messagingSenderId: "798807036118",
    appId: "1:798807036118:web:46fca3fb3ac823a57cb977",
    measurementId: "G-SW7X6X9VPN"
};
firebase.initializeApp(firebaseConfig);

// Retrieve an instance of Firebase Messaging so that it can handle background
// messages.
const messaging = firebase.messaging();

messaging.setBackgroundMessageHandler(function(payload) {
    console.log(
        "[firebase-messaging-sw.js] Received background message ",
        payload,
    );
    // Customize notification here
    const notificationTitle = "Background Message Title";
    const notificationOptions = {
        body: "Background Message body.",
        icon: "/itwonders-web-logo.png",
    };

    return self.registration.showNotification(
        notificationTitle,
        notificationOptions,
    );
});
