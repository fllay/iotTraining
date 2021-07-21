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
    apiKey: "AIzaSyCG59xubOTJe2gMqg1hz2NmcLvY_0AK-_M",
    authDomain: "esp32-b74d2.firebaseapp.com",
    databaseURL: "https://esp32-b74d2-default-rtdb.asia-southeast1.firebasedatabase.app",
    projectId: "esp32-b74d2",
    storageBucket: "esp32-b74d2.appspot.com",
    messagingSenderId: "265654270299",
    appId: "1:265654270299:web:c4088a6dfcebf180ac827e",
    measurementId: "G-SV7SY10B8K"
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
