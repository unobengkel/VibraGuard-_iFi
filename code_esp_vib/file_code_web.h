
String htmlPage = R"====(
<!DOCTYPE html>
<html lang="id">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>VibraGuard WiFi Dashboard</title>
    
    <!-- Bootstrap 5 CSS CDN -->
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css" rel="stylesheet">
    <!-- Font Awesome -->
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.4.0/css/all.min.css">
    <!-- Google Fonts -->
    <link href="https://fonts.googleapis.com/css2?family=Inter:wght@400;700;900&display=swap" rel="stylesheet">

    <style>
        :root {
            --bg-dark: #0a0a0a;
            --glass-bg: rgba(0, 0, 0, 0.6);
            --border-white: rgba(255, 255, 255, 0.1);
        }

        body {
            background-color: var(--bg-dark);
            color: white;
            margin: 0;
            overflow: hidden;
            font-family: 'Inter', sans-serif;
            user-select: none;
        }

        /* Canvas Background */
        canvas {
            display: block;
            position: absolute;
            top: 3%;
            left: 0;
            z-index: 0;
        }

        /* Layer UI */
        .ui-layer {
            position: relative;
            z-index: 10;
            height: 100vh;
            display: flex;
            flex-direction: column;
            pointer-events: none;
        }

        .interactive {
            pointer-events: auto;
        }

        /* Glassmorphism Styles */
        .glass-header {
            background: var(--glass-bg);
            backdrop-filter: blur(10px);
            border-bottom: 1px solid var(--border-white);
            padding: 1rem 1.5rem;
        }

        .glass-panel {
            background: var(--glass-bg);
            backdrop-filter: blur(15px);
            border-top: 1px solid var(--border-white);
            border-radius: 2rem 2rem 0 0;
            padding: 2rem;
        }

        /* Button Styling */
        .btn-vibra {
            border: 1px solid transparent;
            border-radius: 1rem;
            padding: 1.25rem 0.5rem;
            font-weight: bold;
            font-size: 0.75rem;
            transition: all 0.2s ease;
            text-transform: uppercase;
            letter-spacing: 1px;
            color: white;
            display: flex;
            flex-direction: column;
            align-items: center;
            justify-content: center;
        }

        .btn-vibra i {
            font-size: 1.5rem;
            margin-bottom: 0.5rem;
        }

        .btn-vibra:active {
            transform: scale(0.92);
        }

        .btn-on {
            background: linear-gradient(135deg, #059669, #064e3b);
            border-color: rgba(16, 185, 129, 0.3);
        }

        .btn-off {
            background: linear-gradient(135deg, #374151, #111827);
            border-color: rgba(75, 85, 99, 0.5);
        }

        .btn-reset {
            background: linear-gradient(135deg, #dc2626, #7f1d1d);
            border-color: rgba(239, 68, 68, 0.3);
        }

        /* Status Components */
        .status-monitor {
            position: absolute;
            top: 10%;
            width: 100%;
            text-align: center;
        }

        .status-badge {
            display: inline-block;
            padding: 0.25rem 1rem;
            border-radius: 50rem;
            font-size: 0.7rem;
            font-weight: 900;
            letter-spacing: 2px;
            border: 1px solid #444;
            margin-bottom: 1rem;
            transition: all 0.4s ease;
        }

        .main-status-text {
            font-weight: 900;
            font-size: calc(1.5rem + 3vw);
            letter-spacing: -1px;
            transition: color 0.3s ease;
        }

        /* Floating Action Button */
        .fab-sim {
            position: fixed;
            bottom: 180px;
            right: 25px;
            width: 50px;
            height: 50px;
            border-radius: 50%;
            background-color: #2563eb;
            color: white;
            border: none;
            box-shadow: 0 10px 15px -3px rgba(0, 0, 0, 0.4);
            z-index: 100;
        }

        /* Simulation Modal Custom */
        .modal-content-dark {
            background: rgba(17, 24, 39, 0.95);
            backdrop-filter: blur(20px);
            border: 1px solid var(--border-white);
            border-radius: 1.5rem;
            color: white;
        }

        .modal-header { border-bottom: 1px solid var(--border-white); }
        .modal-footer { border-top: 1px solid var(--border-white); }

        .btn-test-warn { background: linear-gradient(to right, #9a3412, #7c2d12); border: 1px solid #ea580c; color: #ffedd5; }
        .btn-test-danger { background: linear-gradient(to right, #991b1b, #7f1d1d); border: 1px solid #ef4444; color: #fee2e2; }

        /* Animation */
        @keyframes pulse-wifi {
            0%, 100% { opacity: 1; }
            50% { opacity: 0.4; }
        }
        .wifi-active {
            color: #10b981;
            animation: pulse-wifi 2s infinite;
        }
    </style>
</head>
<body>

    <!-- LAYER UI -->
    <div class="ui-layer">
        
        <!-- HEADER -->
        <header class="glass-header d-flex justify-content-between align-items-center interactive">
            <div>
                <h1 class="h5 m-0 fw-bold">VibraGuard <span class="text-primary">WiFi</span></h1>
                <small class="text-secondary" style="font-size: 0.65rem;">SECURE LOCAL CONNECTION</small>
            </div>
            <div class="d-flex align-items-center gap-2 wifi-active">
                <i class="fa-solid fa-wifi"></i>
                <span class="fw-bold" style="font-size: 0.7rem; font-family: monospace;">CONNECTED</span>
            </div>
        </header>

        <!-- STATUS MONITOR -->
        <div class="status-monitor interactive">
            <div id="status-badge" class="status-badge text-secondary">
                SYSTEM DISARMED
            </div>
            <div id="main-status-text" class="main-status-text text-secondary">
                NONAKTIF
            </div>
            <p id="last-log" class="small text-secondary mt-2 opacity-0" style="font-family: monospace; font-size: 0.7rem;">
                Last Event: --:--:--
            </p>
        </div>

        <div class="mt-auto glass-panel interactive" style="margin-bottom:105px;">
            <div class="container-fluid p-0">
                <div class="row g-3 justify-content-center mx-auto" style="max-width: 500px;">
                    <!-- Tombol ON -->
                    <div class="col-4">
                        <button onclick="setSystemState('ARMED')" class="btn btn-vibra btn-on w-100">
                            <i class="fa-solid fa-shield-halved"></i>
                            <span>ON</span>
                        </button>
                    </div>
                    <!-- Tombol OFF -->
                    <div class="col-4">
                        <button onclick="setSystemState('DISARMED')" class="btn btn-vibra btn-off w-100">
                            <i class="fa-solid fa-power-off"></i>
                            <span>OFF</span>
                        </button>
                    </div>
                    <!-- Tombol RESET -->
                    <div class="col-4">
                        <button onclick="setSystemState('RESET')" class="btn btn-vibra btn-reset w-100">
                            <i class="fa-solid fa-bell-slash"></i>
                            <span>RESET</span>
                        </button>
                    </div>
                </div>
            </div>
        </div>
    </div>

  

    <canvas id="doorCanvas"></canvas>

    <!-- Bootstrap JS Bundle -->
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/js/bootstrap.bundle.min.js"></script>

    <script>
        // ---------------------------------------------------------
        // CANVAS WIDGET LOGIC (DILARANG UBAH)
        // ---------------------------------------------------------
        const canvas = document.getElementById('doorCanvas');
        const ctx = canvas.getContext('2d');
        let state = 0; 
        let time = 0;

        function resize() {
            canvas.width = window.innerWidth;
            canvas.height = window.innerHeight;
        }
        window.addEventListener('resize', resize);
        resize();

        function changeState(s) {
            state = s;
        }

        function drawDoor() {
            const w = 240; const h = 380;
            const x = canvas.width / 2 - w / 2;
            const y = canvas.height / 2 - h / 2;
            ctx.save();
            if (state === 2) {
                ctx.translate((Math.random() - 0.5) * 12, (Math.random() - 0.5) * 12);
            } else if (state === 1) {
                ctx.translate(Math.sin(time * 0.5) * 2, 0);
            }
            ctx.shadowBlur = state === 0 ? 20 : (state === 1 ? 40 : 70);
            ctx.shadowColor = state === 0 ? 'rgba(37, 99, 235, 0.5)' : (state === 1 ? 'rgba(249, 115, 22, 0.7)' : 'rgba(220, 38, 38, 0.9)');
            ctx.fillStyle = '#171717';
            ctx.fillRect(x - 10, y - 10, w + 20, h + 20);
            const gradient = ctx.createLinearGradient(x, y, x + w, y);
            gradient.addColorStop(0, '#262626'); gradient.addColorStop(0.5, '#404040'); gradient.addColorStop(1, '#262626');
            ctx.fillStyle = gradient; ctx.shadowBlur = 0; 
            ctx.fillRect(x, y, w, h);
            ctx.strokeStyle = 'rgba(255,255,255,0.05)'; ctx.lineWidth = 2;
            for(let i = 0; i < 5; i++) { ctx.strokeRect(x + 15, y + 15 + (i * 70), w - 30, 60); }
            const pulse = Math.sin(time * 0.1) * 0.5 + 0.5;
            const alpha = state === 0 ? 0.3 : (state === 1 ? 0.4 + pulse * 0.4 : 0.6 + pulse * 0.4);
            ctx.fillStyle = `rgba(0,0,0,0.8)`; ctx.beginPath(); ctx.arc(x + w/2, y + h/2, 40, 0, Math.PI * 2); ctx.fill();
            ctx.strokeStyle = state === 0 ? `rgba(37, 99, 235, ${alpha})` : (state === 1 ? `rgba(249, 115, 22, ${alpha})` : `rgba(220, 38, 38, ${alpha})`);
            ctx.lineWidth = 4; ctx.beginPath(); ctx.arc(x + w/2, y + h/2, 35, 0, Math.PI * 2); ctx.stroke();
            ctx.fillStyle = ctx.strokeStyle; ctx.font = 'bold 24px Arial'; ctx.textAlign = 'center';
            const icon = state === 0 ? "🔒" : (state === 1 ? "⚠️" : "⚡");
            ctx.fillText(icon, x + w/2, y + h/2 + 8);
            ctx.restore();
        }

        function drawBackground() {
            ctx.fillStyle = '#0a0a0a'; ctx.fillRect(0, 0, canvas.width, canvas.height);
            ctx.strokeStyle = 'rgba(255,255,255,0.02)'; ctx.lineWidth = 1;
            for(let i=0; i<canvas.width; i+=50) { ctx.beginPath(); ctx.moveTo(i,0); ctx.lineTo(i, canvas.height); ctx.stroke(); }
            for(let i=0; i<canvas.height; i+=50) { ctx.beginPath(); ctx.moveTo(0,i); ctx.lineTo(canvas.width, i); ctx.stroke(); }
            if (state === 2) {
                const vigGrad = ctx.createRadialGradient(canvas.width/2, canvas.height/2, 100, canvas.width/2, canvas.height/2, canvas.width);
                vigGrad.addColorStop(0, 'transparent'); vigGrad.addColorStop(1, `rgba(153, 27, 27, ${0.2 + Math.sin(time*0.2)*0.1})`);
                ctx.fillStyle = vigGrad; ctx.fillRect(0,0, canvas.width, canvas.height);
            }
        }

        function animate() {
            time++; drawBackground(); drawDoor(); requestAnimationFrame(animate);
        }
        animate();

        // ---------------------------------------------------------
        // VIBRAGUARD LOGIC (INTEGRASI & BOOTSTRAP UI)
        // ---------------------------------------------------------
        const badgeEl = document.getElementById('status-badge');
        const mainTextEl = document.getElementById('main-status-text');
        const logEl = document.getElementById('last-log');

        let isSystemArmed = false;
        let isAlarmTriggered = false;
        var ipesp = "/";

        function updateUI(badgeText, mainText, colorCode, borderCode, bgCode) {
            badgeEl.innerText = badgeText;
            badgeEl.style.color = colorCode;
            badgeEl.style.borderColor = borderCode;
            badgeEl.style.backgroundColor = bgCode;

            mainTextEl.innerText = mainText;
            mainTextEl.style.color = colorCode;
        }

        function setStatusAlarmMute() {
            isSystemArmed = false;
            changeState(0);
            updateUI("SYSTEM DISARMED", "NONAKTIF", "#6c757d", "#444", "rgba(33, 37, 41, 0.8)");
        }

        function setStatusAlarmNoMute() {
            isSystemArmed = true;
            changeState(0);
            updateUI("SYSTEM ARMED", "SIAGA (STANDBY)", "#10b981", "#10b981", "rgba(6, 78, 59, 0.5)");
        }

        function setDoorBahaya() {
            const now = new Date().toLocaleTimeString();
            isAlarmTriggered = true;
            changeState(2);
            updateUI("ALARM TRIGGERED!", "ADA GETARAN!", "#ef4444", "#ef4444", "rgba(127, 29, 29, 0.9)");
            logEl.innerText = `Last Event: ${now}`;
            logEl.classList.remove('opacity-0');
        }

        function setDoorPeringatan() {
            const now = new Date().toLocaleTimeString();
            isAlarmTriggered = true;
            changeState(1);
            updateUI("SYSTEM WARNING", "MENCURIGAKAN", "#f59e0b", "#f59e0b", "rgba(120, 53, 15, 0.9)");
            logEl.innerText = `Last Warning: ${now}`;
            logEl.classList.remove('opacity-0');
        }

        function setSystemState(action) {
            if (action === 'DISARMED') { setMute(); setStatusAlarmMute(); }
            else if (action === 'ARMED') { setNoMute(); setStatusAlarmNoMute(); }
            else if (action === 'RESET') { setReset(); }
        }

        function simulateState(level) {
            if (!isSystemArmed) {
                const toast = document.createElement('div');
                toast.className = 'fixed-top mt-5 pt-5 text-center';
                toast.innerHTML = '<span class="badge bg-danger p-3">Sistem belum aktif (OFF). Silakan tekan tombol ON dahulu.</span>';
                document.body.appendChild(toast);
                setTimeout(() => toast.remove(), 2500);
                return;
            }
            if (level === 1) setDoorPeringatan();
            else if (level === 2) setDoorBahaya();
            
            // Tutup modal secara otomatis menggunakan Bootstrap API
            const modalEl = document.getElementById('simModal');
            const modal = bootstrap.Modal.getInstance(modalEl);
            modal.hide();
        }

        // ---------------------------------------------------------
        // AJAX CALLS TO ESP
        // ---------------------------------------------------------
        function sendRequest(endpoint) {
            var xhr = new XMLHttpRequest();
            xhr.open("GET", ipesp + endpoint, true);
            xhr.onreadystatechange = function() {
                if (this.readyState == 4 && this.status == 200) {
                    console.log("Response from " + endpoint + ": " + this.responseText);
                }
            };
            xhr.send();
        }

        function setReset() { 
            sendRequest("reset"); 
            if (isSystemArmed) setStatusAlarmNoMute(); else setStatusAlarmMute();
        }
        function setNoMute() { sendRequest("setnomute"); }
        function setMute() { sendRequest("setmute"); }

        function loadData() {
            var xhr = new XMLHttpRequest();
            xhr.open("GET", ipesp + "get", true);
            xhr.timeout = 1500;
            xhr.onreadystatechange = function() {
                if (this.readyState == 4 && this.status == 200) {
                    var data = this.responseText.split('x');
                    var waspadaReported = parseFloat(data[1]);
                    var buzzerAktif = parseFloat(data[4]);
                    var isMute = parseFloat(data[3]);

                    if(waspadaReported == 1) setDoorPeringatan();
                    else if(buzzerAktif == 1) setDoorBahaya();
                    else {
                        if(isMute == 1) setStatusAlarmMute();
                        else setStatusAlarmNoMute();
                    }
                }
            };
            xhr.send();
        }

        setInterval(loadData, 500);
        setStatusAlarmMute(); // Init awal

    </script>
</body>
</html>
)====";


