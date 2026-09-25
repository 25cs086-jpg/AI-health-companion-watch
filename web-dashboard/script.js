const state = {
  heartRate: 78,
  fall: false,
  emergency: false,
  pendingResponse: false
};

const $ = (id) => document.getElementById(id);

function render() {
  $("heartRate").textContent = state.heartRate;

  $("fallStatus").textContent = state.fall ? "DETECTED" : "SAFE";
  $("emergencyStatus").textContent = state.emergency ? "ALERT SENT" : "INACTIVE";

  $("overallStatus").textContent =
    state.emergency ? "Emergency Alert Triggered" :
    state.fall ? "Possible Fall Detected" :
    "System Normal";

  $("statusText").textContent =
    state.emergency ? "Guardian communication has been triggered." :
    state.fall ? "The watch is waiting for the user's response." :
    "Continuous monitoring is active.";

  $("alertBox").classList.toggle("hidden", !state.pendingResponse);

  $("fallEvent").querySelector("small").textContent =
    state.fall ? "Fall detected — asking user for confirmation" : "Waiting for event";

  $("guardianEvent").querySelector("small").textContent =
    state.emergency ? "Emergency alert triggered" : "Waiting for emergency";

  if (state.emergency) {
    $("alertTitle").textContent = "Emergency Alert Triggered";
    $("alertMessage").textContent = "No user confirmation received. Guardian communication should be initiated.";
  } else {
    $("alertTitle").textContent = "Are you okay?";
    $("alertMessage").textContent = "A possible fall was detected. Press the button to confirm.";
  }
}

function normal() {
  state.heartRate = 78;
  state.fall = false;
  state.emergency = false;
  state.pendingResponse = false;
  render();
}

function simulateFall() {
  state.heartRate = 92;
  state.fall = true;
  state.emergency = false;
  state.pendingResponse = true;
  render();
}

function simulateEmergency() {
  state.heartRate = 104;
  state.fall = true;
  state.emergency = true;
  state.pendingResponse = false;
  render();
}

$("normalBtn").addEventListener("click", normal);
$("fallBtn").addEventListener("click", simulateFall);
$("emergencyBtn").addEventListener("click", simulateEmergency);
$("resetBtn").addEventListener("click", normal);

$("okButton").addEventListener("click", () => {
  state.fall = false;
  state.emergency = false;
  state.pendingResponse = false;
  state.heartRate = 80;
  render();
});

// Small simulated heart-rate fluctuation for a more realistic dashboard.
setInterval(() => {
  if (!state.emergency) {
    const delta = Math.floor(Math.random() * 5) - 2;
    state.heartRate = Math.max(60, Math.min(110, state.heartRate + delta));
    $("heartRate").textContent = state.heartRate;
  }
}, 2500);

render();
