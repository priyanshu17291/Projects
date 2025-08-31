class Chessboard {
    sparePieces = false;
    legalMoveOnly = false;
    moves = [];
    turn = "w";
    status = "active";
    PGN = [];
    FENs = [];
    #pieceTheme = "neo";
    #moveStack = [];
    #board;
    #undo = false;
    #chessboard;
    #format = "png";
    #highlightSquare;
    #isDragging = false;
    #highlightLegalMoves = false;
    #darkColor = "rgb(115,149,82)";
    #lightColor = "rgb(235,236,208)";
    #BRIDGE = null;
    #halfMoveCount = 0;
    #fullMoveCount = 1;
    #enPassantSquare = "";
    #FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    #timeFormat = null;
    #whiteTime = 0;
    #blackTime = 0;
    #increment = 0;
    #timerInterval = null;
    #lastTimestamp = 0;
    #ID1 = "";
    #ID2 = "";
    #matchStarted = false;
    #enableSound = false;
    #isAnimating = false;
    #castlingRights = { w: ["", ""], b: ["", ""] };
    #promotingPiece = "q";
    #width;
    #offsetX;
    #offsetY;
    #firstClick = 0;
    #draggableDiv = null;
    #originalParent = null;
    #startSquare = "";
    #endSquare = "";
    #lastMove = { start: "", end: "" };
    #pieceMap = {
        bp: "blackPawn",
        wp: "whitePawn",
        bb: "blackBishop",
        wb: "whiteBishop",
        bn: "blackKnight",
        wn: "whiteKnight",
        br: "blackRook",
        wr: "whiteRook",
        bq: "blackQueen",
        wq: "whiteQueen",
        bk: "blackKing",
        wk: "whiteKing",
    };
    #_pieceMap = {
        blackPawn: "bp",
        whitePawn: "wp",
        blackBishop: "bb",
        whiteBishop: "wb",
        blackKnight: "bn",
        whiteKnight: "wn",
        blackRook: "br",
        whiteRook: "wr",
        blackQueen: "bq",
        whiteQueen: "wq",
        blackKing: "bk",
        whiteKing: "wk",
    };
    #fenChar = {
        p: "bp",
        r: "br",
        n: "bn",
        b: "bb",
        q: "bq",
        k: "bk",
        P: "wp",
        R: "wr",
        N: "wn",
        B: "wb",
        Q: "wq",
        K: "wk",
    };
    #formatTime(milliseconds) {
        const totalSeconds = Math.floor(milliseconds / 1000);
        const hours = Math.floor(totalSeconds / 3600);
        const minutes = Math.floor((totalSeconds % 3600) / 60);
        const seconds = totalSeconds % 60;
        const deciseconds = Math.floor((milliseconds % 1000) / 100);
        if (hours > 0) {
            return `${hours.toString().padStart(2, "0")}:${minutes.toString().padStart(2, "0")}:${seconds.toString().padStart(2, "0")}.${deciseconds}`;
        } else {
            return `${minutes.toString().padStart(2, "0")}:${seconds.toString().padStart(2, "0")}.${deciseconds}`;
        }
    }
    #_fenChar = {
        bp: "p",
        br: "r",
        bn: "n",
        bb: "b",
        bq: "q",
        bk: "k",
        wp: "P",
        wr: "R",
        wn: "N",
        wb: "B",
        wq: "Q",
        wk: "K",
    };
    constructor(id, options = {}) {
        this.#board = document.getElementById(id);
        if (!this.#board) {
            throw new Error("Element not found");
        }
        this.#addStyles();
        this.#board.classList.add("board");
        this.#board.innerHTML += this.#innerHTML;
        document.addEventListener('DOMContentLoaded', () => {
            const modal = document.getElementById('myModal');
            const span = document.querySelector('.close');

            // btn.onclick = () => modal.style.display = 'block';
            span.onclick = () => modal.style.display = 'none';
            window.onclick = event => {
                if (event.target === modal) modal.style.display = 'none';
            }
        });
        if (typeof options === "string") {
            if (options !== "start") {
                this.#FEN = options;
            }
        } else if (typeof options === "object") {
            if (options.position && options.position !== "start") {
                this.#FEN = options.position;
            }
            const data = this.#FEN.split(" ");
            this.turn = data[1];
            if (data[2].includes("K")) {
                this.#castlingRights["w"][0] = "K";
            }
            if (data[2].includes("Q")) {
                this.#castlingRights["w"][1] = "Q";
            }
            if (data[2].includes("k")) {
                this.#castlingRights["b"][0] = "k";
            }
            if (data[2].includes("q")) {
                this.#castlingRights["b"][1] = "q";
            }
            if (data[3] !== "-")
                this.#enPassantSquare = this.convertCoordtoId(data[3]);
            this.#halfMoveCount = parseInt(data[4]);
            this.#fullMoveCount = parseInt(data[5]);
            this.orientation = options.orientation || "white";
            this.draggable =
                options.draggable !== undefined ? options.draggable : false;
            this.#format = options.format || "pgn";
            this.#timeFormat = options.timeFormat || null;
            this.showNotation =
                options.showNotation !== undefined ? options.showNotation : false;
            this.#pieceTheme = options.pieceTheme || "neo";
            this.legalMoveOnly = options.onlyLegalMoves || false;
            this.#darkColor = options.darkColor || "rgb(115,149,82)";
            this.#lightColor = options.lightColor || "rgb(235,236,208)";
            this.#highlightLegalMoves = options.highlightLegalMoves || false;
            this.legalMoveOnly = options.onlyLegalMoves || false;
            this.#enableSound = options.enableSound || false;
        } else {
            const data = this.#FEN.split(" ");
            this.turn = data[1];
            if (data[2].includes("K")) {
                this.#castlingRights["w"][0] = "K";
            }
            if (data[2].includes("Q")) {
                this.#castlingRights["w"][1] = "Q";
            }
            if (data[2].includes("k")) {
                this.#castlingRights["b"][0] = "k";
            }
            if (data[2].includes("q")) {
                this.#castlingRights["b"][1] = "q";
            }
            if (data[3] !== "-")
                this.#enPassantSquare = this.convertCoordtoId(data[3]);
            this.#halfMoveCount = parseInt(data[4]);
            this.#fullMoveCount = parseInt(data[5]);
        }
        if (this.#timeFormat) {
            this.#whiteTime = this.#timeFormat.minute * 60 * 1000;
            this.#blackTime = this.#timeFormat.minute * 60 * 1000;
            this.#increment = this.#timeFormat.increment * 1000;
            this.#ID1 = this.#timeFormat.ID1;
            this.#ID2 = this.#timeFormat.ID2;
            this.updateTimer();
            this.#matchStarted = true;
            setTimeout(() => {
                this.#startTimer();
                document.addEventListener("DOMContentLoaded", () => {
                    this.#playSound("start");
                });
            }, 1000);
        }
        this.#width = this.#board.offsetWidth;
        this.#chessboard = document.createElement("div");
        this.#chessboard.classList.add("chessboard");
        this.#board.appendChild(this.#chessboard);
        this.#highlightSquare = "hovered";
        this.handleKeyDown = this.#handleKeyDown.bind(this);
        this.onScroll = this.#onScroll.bind(this);
        document.addEventListener("keydown", this.#handleKeyDown);
        this.#chessboard.addEventListener("wheel", this.#onScroll);
        this.#initializeBoard(this.#FEN);
        this.FENs.push(this.#FEN);
        if (this.draggable) {
            this.enableDrag();
        }
    }
    bridge(callback) {
        this.#BRIDGE = callback;
    }
    #innerHTML = `
            <div id="myModal" class="modal-gamestatus">
            <div class="modal-content">
                <span class="close">&times;</span>
                <p>lorem</p>
                <div id="modal-features">
                    <button id="rematch" class="modal-options">Rematch</button>
                    <button id="newgame" class="modal-options">New Game</button>
                </div>
            </div>
        </div>`

    #addStyles() {
        let e = `
        .board{display:flex;flex-direction:column;justify-content:center;
        position:relative} .chessboard{aspect-ratio:1/1;display:flex;
        flex-wrap:wrap;border-radius:.25em;overflow:hidden;position:relative}
        .square{width:calc(100%/8);height:calc(100%/8);cursor:grab;position:relative;
        display:flex;justify-content:center;align-items:center}.sparePieces{width:calc(600%/8);
        aspect-ratio:4/1}.sparePieces img{aspect-ratio:1/1;width:calc(90%/6);
        cursor:grab}.chessPiece{width:90%;aspect-ratio:1/1;cursor:grab;z-index:5;
        transition:transform .5s ease-in;position:relative}.square.hovered{box-shadow:inset 0 0 .6em #63ea4b
        }.dragging{cursor:grabbing;z-index:1000}.buttons{height:50px;width:50px}.file{display:flex;
        justify-content:center;align-items:center;width:25%;height:25%;font-family:Arial,Helvetica,sans-serif
        ;font-size:.7em;font-weight:600;position:absolute;bottom:2%;right:0}.rank{display:flex
        ;justify-content:center;align-items:center;width:25%;height:25%;font-size:.8em;font-weight:600
        ;position:absolute;top:5%;left:0}.fade-out{animation:fadeOut .6s forwards}@keyframes 
        fadeOut{from{opacity:1}to{opacity:0}}.square.analysisW{background-color:#EB7D6A!important}
        .square.analysisB{background-color:#D36C50!important}.circle{width:35%;height:35%;border-radius:50%;
        background-color:black;opacity:.2}.ring{width:80%;height:80%;border:.25em solid#000000;border-radius:50%
        ;opacity:.2;position:absolute;top:50%;left:50%;transform:translate(-50%,-50%)}
        .board,.image{-webkit-user-select:none;-moz-user-select:none;-ms-user-select:none
        ;user-select:none}.modal-gamestatus{user-select:none;display:none;position:absolute
        ;z-index:10;margin:auto;width:100%;height:100%;background:rgba(0,0,0,0.4);animation:fadeIn .5s}
        .modal-gamestatus .modal-content{background:#fff;margin:15% auto;padding:20px;border-radius:10px
        ;width:40%;height:60%;max-width:500px;animation:slideDown .5s forwards;display:flex;
        flex-direction:column;justify-content:space-around;span{color:#000;font-size:xx-large
        ;text-align:end;cursor:pointer;font-weight:900}p{text-align:center;font-size:1em;font-weight:600;
        width:80%;font-family:cursive;height:50%;margin:0 auto;display:flex;justify-content:center;align-items:center}
        #modal-features{display:flex;justify-content:space-around;.modal-options{color:white;width:42%;height:3em;
        background-color:#636261;margin:auto 0;font-size:x-small;font-family:cursive;display:flex;cursor:pointer;
        border-radius:5px;text-align:center;align-items:center;justify-content:center}}}@keyframes fadeIn{from{opacity:0}
        to{opacity:1}}@keyframes slideDown{from{transform:translateY(-100%)}to{transform:translateY(0)}}
  `,
            t = document.querySelector("style");
        t || ((t = document.createElement("style")), document.head.appendChild(t)),
            t.appendChild(document.createTextNode(e));
    }

    #startTimer() {
        this.#lastTimestamp = performance.now();
        this.#timerInterval = setInterval(() => this.updateTimer(), 100);
    }
    #stopTimer() {
        clearInterval(this.#timerInterval);
    }
    updateTimer() {
        const now = performance.now();
        const elapsed = now - this.#lastTimestamp;
        this.#lastTimestamp = now;
        if (this.turn === "w") {
            this.#whiteTime -= elapsed;
            if (this.#whiteTime < 0) {
                this.#whiteTime = 0;
                this.#stopTimer();
                alert("White lost on time");
            }
        } else {
            this.#blackTime -= elapsed;
            if (this.#blackTime < 0) {
                this.#blackTime = 0;
                this.#stopTimer();
                alert("Black lost on time");
            }
        }
        this.#updateDisplay();
    }
    #toggleTimer() {
        if (!this.#matchStarted) return;
        this.#stopTimer();
        if (this.turn === "b") {
            this.#whiteTime += this.#increment;
        } else {
            this.#blackTime += this.#increment;
        }
        this.#startTimer();
    }
    #updateDisplay() {
        const result = [
            this.#formatTime(this.#whiteTime),
            this.#formatTime(this.#blackTime),
        ];
        let a = document.getElementById(`${this.#ID1}`);
        if (a) a.textContent = result[0];
        let b = document.getElementById(`${this.#ID2}`);
        if (b) b.textContent = result[1];
        return result;
    }
    #onScroll(event) {
        event.preventDefault();
        if (event.deltaY < 0 && !this.#isAnimating) {
            this.undoMove();
        }
    }
    #handleKeyDown(event) {
        if (event.key === "ArrowRight") {
            this.#handleRightArrow();
        } else if (event.key === "ArrowLeft") {
            this.#handleLeftArrow();
        }
    }
    #handleRightArrow() {
        console.log("Right arrow key pressed");
    }
    #handleLeftArrow() {
        this.undoMove();
        console.log("Left arrow key pressed");
    }
    #initializeBoard(position) {
        for (let i = 1; i <= 8; i++) {
            for (let j = 1; j <= 8; j++) {
                const square = document.createElement("div");
                square.style.backgroundColor =
                    (i + j) % 2 === 0 ? this.#lightColor : this.#darkColor;
                square.classList.add("square", (i + j) % 2 === 0 ? "white" : "black");
                square.id = "sq" + (8 * (i - 1) + j);
                square.addEventListener("click", () => {
                    if (square.childNodes.length === 0) { this.#hideLegalMoves(); }
                    if (this.legalMoveOnly && this.moves.includes(square.id)) {
                        this.#endSquare = square.id;
                        this.makeMove(this.#startSquare, this.#endSquare, "byClick");
                    } else if (!this.legalMoveOnly) {
                        this.#endSquare = square.id;
                        this.makeMove(this.#startSquare, this.#endSquare, "byClick");
                    }
                });
                this.#chessboard.appendChild(square);
            }
        }
        this.#chessboard.querySelectorAll(".black").forEach((square) => {
            square.addEventListener("contextmenu", (event) => {
                event.preventDefault();
                if (!square.classList.contains("analysisB")) {
                    square.classList.add("analysisB");
                } else {
                    square.classList.remove("analysisB");
                }
            });
        });
        this.#chessboard.querySelectorAll(".white").forEach((square) => {
            square.addEventListener("contextmenu", (event) => {
                event.preventDefault();
                if (!square.classList.contains("analysisW")) {
                    square.classList.add("analysisW");
                } else {
                    square.classList.remove("analysisW");
                }
            });
        });
        if (position === "start") {
            this.start();
        } else if (position != "") {
            this.#setPosition(position);
        }
        if (this.showNotation) {
            this.#enableCoordinates();
        }
        if (this.orientation === "black") {
            this.flipBoard();
        }
    }
    #enableCoordinates() {
        this.showNotation = true;
        if (!this.#chessboard.children[0].querySelector("span")) {
            const files = ["a", "b", "c", "d", "e", "f", "g", "h"];
            const colorF = this.#chessboard.children[0].id === "sq1" ? "w" : "b";
            for (let i = 0; i < 8; i++) {
                let idx = colorF === "w" ? 1 : -1;
                let index = colorF === "w" ? i : 7 - i;
                let st = colorF === "w" ? 57 : 8;
                const file = document.createElement("span");
                file.classList.add("file");
                file.style.color = i % 2 === 0 ? this.#lightColor : this.#darkColor;
                file.innerText = files[index];
                let sqr = this.#chessboard.querySelector(
                    `.square[id="sq${st + i * idx}"]`
                );
                sqr.appendChild(file);
            }
            const colorR = this.#chessboard.children[0].id === "sq1" ? 57 : 8;
            for (let i = 0; i < 8; i++) {
                let idx = colorR === 8 ? 8 : -8;
                const rank = document.createElement("span");
                rank.classList.add("rank");
                rank.innerText = colorR === 57 ? i + 1 : 8 - i;
                rank.style.color = i % 2 === 0 ? this.#lightColor : this.#darkColor;
                let sqr = this.#chessboard.querySelector(
                    `.square[id="sq${idx * i + colorR}"]`
                );
                sqr.appendChild(rank);
            }
        }
        return;
    }
    #disableCoordinates() {
        this.showNotation = false;
        const files = this.#chessboard.querySelectorAll(".file");
        const ranks = this.#chessboard.querySelectorAll(".rank");
        files.forEach((file) => file.remove());
        ranks.forEach((rank) => rank.remove());
    }
    #createImage(pieceCode) {
        const img = document.createElement("img");
        img.src = `media/chess-pieces/${this.#pieceTheme}/${pieceCode}.${this.#format}`;
        img.draggable = false;
        img.alt = "";
        img.classList.add(this.#pieceMap[pieceCode], "chessPiece", pieceCode);
        return img;
    }
    #createCircle(square) {
        let circle = document.createElement("div");
        circle.className = "circle";
        if (square) square.appendChild(circle);
    }
    #createRing(square) {
        let ring = document.createElement("div");
        ring.className = "ring";
        if (square) {
            square.appendChild(ring);
        }
    }
    #showLegalMoves(moves, fen) {
        if (!this.#highlightLegalMoves) return;
        const board = this.#fenToBoard(fen).flat();
        let squares = moves.map((move) => parseInt(move.match(/\d+/)[0]) - 1);
        squares.forEach((move) => {
            const square = board[move];
            if (square) {
                let sq = this.#chessboard.querySelector(`#sq${move + 1}`);
                this.#createRing(sq);
            } else if (
                !square &&
                this.#enPassantSquare &&
                move === parseInt(this.#enPassantSquare.match(/\d+/)[0]) - 1
            ) {
                let sq = this.#chessboard.querySelector(`#sq${move + 1}`);
                this.#createRing(sq);
            } else {
                let sq = this.#chessboard.querySelector(`#sq${move + 1}`);
                this.#createCircle(sq);
            }
        });
    }
    #hideLegalMoves() {
        if (!this.#highlightLegalMoves) return;
        this.#chessboard
            .querySelectorAll(".circle")
            .forEach((circle) => circle.remove());
        this.#chessboard.querySelectorAll(".ring").forEach((ring) => ring.remove());
    }
    #changeTurn() {
        this.turn = this.turn === "w" ? "b" : "w";
        if (this.#timeFormat) this.#toggleTimer();
    }
    #nextTurn() {
        return this.turn === "w" ? "b" : "w";
    }
    #playSound(move) {
        if (!this.#enableSound) return;
        let audio;
        switch (move) {
            case "start":
                audio = new Audio("media/audios/game-start.mp3");
                audio.play();
                break;
            case "move":
                audio = new Audio("media/audios/move-self.mp3");
                audio.play();
                break;
            case "capture":
                audio = new Audio("media/audios/capture.mp3");
                audio.play();
                break;
            case "castle":
                audio = new Audio("media/audios/castle.mp3");
                audio.play();
                break;
            case "promotion":
                audio = new Audio("media/audios/promote.mp3");
                audio.play();
                break;
            case "check":
                audio = new Audio("media/audios/move-check.mp3");
                audio.play();
                break;
            case "checkmate":
                audio = new Audio("media/audios/game-end.mp3");
                audio.play();
                break;
            default:
                break;
        }
    }
    #hideAnalysis() {
        this.#chessboard.querySelectorAll(".square").forEach((square) => {
            if (
                square.classList.contains("analysisB") ||
                square.classList.contains("analysisW")
            ) {
                square.classList.remove("analysisB");
                square.classList.remove("analysisW");
            }
        });
    }
    #moveCond(image, SqId) {
        if (this.legalMoveOnly) {
            if (image.classList[2][0] === this.turn) {
                this.moves = this.#validMoves(SqId);
                return true;
            }
            return false;
        }
        this.moves.length = 0;
        this.moves.push(...Array.from({ length: 64 }, (_, i) => "sq" + (i + 1)));
        return true;
    }
    #startDrag(square, e) {
        this.#hidePrompt();
        if ((e.button === 0 || e.touches) && this.draggable) {
            this.#hideAnalysis();
            const clientX = e.clientX || e.touches[0].clientX;
            const clientY = e.clientY || e.touches[0].clientY;
            const scrollLeft =
                window.pageXOffset || document.documentElement.scrollLeft;
            const scrollTop =
                window.pageYOffset || document.documentElement.scrollTop;
            const image = square.querySelector(".chessPiece");
            if (image && this.#moveCond(image, square.id)) {
                if (this.#highlightLegalMoves) {
                    this.#hideLegalMoves();
                    this.#showLegalMoves(this.moves, this.fen());
                }
                if (this.#startSquare === square.id) {
                    this.#firstClick = 1;
                } else {
                    this.#firstClick = 0;
                }
                this.#startSquare = square.id;
                this.#isDragging = true;
                this.#draggableDiv = image;
                this.#originalParent = this.#draggableDiv.parentElement;
                this.#originalParent.classList.add(this.#highlightSquare);
                let rect = this.#draggableDiv.getBoundingClientRect();
                this.#offsetX = clientX - (rect.left + rect.width / 2) + scrollLeft;
                this.#offsetY = clientY - (rect.top + rect.height / 2) + scrollTop;
                document.body.appendChild(this.#draggableDiv);
                this.#draggableDiv.style.position = "absolute";
                this.#draggableDiv.style.width = rect.width + "px";
                this.#draggableDiv.style.height = rect.height + "px";
                this.#draggableDiv.style.left =
                    clientX - rect.width / 2 + scrollLeft + "px";
                this.#draggableDiv.style.top =
                    clientY - rect.height / 2 + scrollTop + "px";
                this.#draggableDiv.style.cursor = "grabbing";
            }
        }
    }
    #moveDrag(e) {
        if (e.button === 0 || e.touches) {
            const clientX = e.clientX || e.touches[0].clientX;
            const clientY = e.clientY || e.touches[0].clientY;
            let scrollLeft =
                window.pageXOffset || document.documentElement.scrollLeft;
            let scrollTop = window.pageYOffset || document.documentElement.scrollTop;
            let chessboardRect = this.#chessboard.getBoundingClientRect();
            if (
                clientX >= chessboardRect.left &&
                clientX <= chessboardRect.right &&
                clientY >= chessboardRect.top &&
                clientY <= chessboardRect.bottom
            ) {
                e.preventDefault();
            }
            if (this.#isDragging && this.#draggableDiv) {
                let newLeft = clientX - this.#draggableDiv.offsetWidth / 2 + scrollLeft;
                let newTop = clientY - this.#draggableDiv.offsetHeight / 2 + scrollTop;
                if (newLeft < chessboardRect.left + scrollLeft) {
                    newLeft = chessboardRect.left + scrollLeft;
                } else if (
                    newLeft + this.#draggableDiv.offsetWidth >
                    chessboardRect.right + scrollLeft
                ) {
                    newLeft =
                        chessboardRect.right + scrollLeft - this.#draggableDiv.offsetWidth;
                }
                if (newTop < chessboardRect.top + scrollTop) {
                    newTop = chessboardRect.top + scrollTop;
                } else if (
                    newTop + this.#draggableDiv.offsetHeight >
                    chessboardRect.bottom + scrollTop
                ) {
                    newTop =
                        chessboardRect.bottom + scrollTop - this.#draggableDiv.offsetHeight;
                }
                this.#draggableDiv.style.left = newLeft + "px";
                this.#draggableDiv.style.top = newTop + "px";
                let targetBox = null;
                let boxes = this.#chessboard.querySelectorAll(".square");
                boxes.forEach((box) => {
                    let rect = box.getBoundingClientRect();
                    if (
                        clientX >= rect.left &&
                        clientX <= rect.right &&
                        clientY >= rect.top &&
                        clientY <= rect.bottom
                    ) {
                        targetBox = box;
                    }
                    box.classList.remove(this.#highlightSquare);
                });
                if (targetBox) {
                    targetBox.classList.add(this.#highlightSquare);
                }
            }
        }
    }
    #endDrag(e) {
        if (e.button === 0 || (e.changedTouches && e.changedTouches[0])) {
            const clientX =
                e.clientX || (e.changedTouches && e.changedTouches[0].clientX);
            const clientY =
                e.clientY || (e.changedTouches && e.changedTouches[0].clientY);
            let scrollLeft =
                window.pageXOffset || document.documentElement.scrollLeft;
            let scrollTop = window.pageYOffset || document.documentElement.scrollTop;
            if (this.#isDragging && this.#draggableDiv) {
                this.#isDragging = false;
                this.#draggableDiv.style.cursor = "grab";
                let targetBox = null;
                let boxes = this.#chessboard.querySelectorAll(".square");
                boxes.forEach((box) => {
                    let rect = box.getBoundingClientRect();
                    if (
                        clientX >= rect.left &&
                        clientX <= rect.right &&
                        clientY >= rect.top &&
                        clientY <= rect.bottom
                    ) {
                        targetBox = box;
                    }
                    box.classList.remove(this.#highlightSquare);
                });
                if (targetBox && this.moves.includes(targetBox.id)) {
                    this.#endSquare = targetBox.id;
                    this.makeMove(this.#startSquare, this.#endSquare, "byDrag");
                } else {

                    this.#originalParent.appendChild(this.#draggableDiv);
                    this.#draggableDiv.style.position = "relative";
                    this.#draggableDiv.style.left = 0;
                    this.#draggableDiv.style.top = 0;
                    this.#originalParent.classList.remove(this.#highlightSquare);
                    this.#originalParent = null;
                }
            }
        }
    }
    #initializeDragAndDrop() {
        this.boundStartDrag = (e) => this.#startDrag(e.currentTarget, e);
        this.boundMoveDrag = this.#moveDrag.bind(this);
        this.boundEndDrag = this.#endDrag.bind(this);
        this.#chessboard.querySelectorAll(".square").forEach((square) => {
            square.addEventListener("mousedown", this.boundStartDrag);
            square.addEventListener("touchstart", this.boundStartDrag);
        });
        document.addEventListener("mousemove", this.boundMoveDrag);
        document.addEventListener("touchmove", this.boundMoveDrag, {
            passive: false,
        });
        document.addEventListener("mouseup", this.boundEndDrag);
        document.addEventListener("touchend", this.boundEndDrag);
        document.addEventListener("dragstart", (e) => {
            if (e.target.classList.contains("chessPiece")) {
                e.preventDefault();
            }
        });
    }
    #disableDragAndDrop() {
        this.#chessboard.querySelectorAll(".square").forEach((square) => {
            square.removeEventListener("mousedown", this.boundStartDrag);
            square.removeEventListener("touchstart", this.boundStartDrag);
        });
        document.removeEventListener("mousemove", this.boundMoveDrag);
        document.removeEventListener("touchmove", this.boundMoveDrag);
        document.removeEventListener("mouseup", this.boundEndDrag);
        document.removeEventListener("touchend", this.boundEndDrag);
    }
    enableDrag() {
        this.#initializeDragAndDrop();
    }
    disableDrag() {
        this.#disableDragAndDrop();
    }
    sparePieces(permission) {
        if (permission) {
            const sparePieces = document.createElement("div");
            sparePieces.classList.add("sparePieces");
            const pieces = [
                "wp",
                "wb",
                "wn",
                "wr",
                "wq",
                "wk",
                "bp",
                "bb",
                "bn",
                "br",
                "bq",
                "bk",
            ];
            pieces.forEach((piece) => {
                if (piece) {
                    const img = this.#createImage(piece);
                    sparePieces.appendChild(img);
                }
            });
            this.boundStartDrag = (e) => this.#startDrag(e.currentTarget, e);
            this.boundMoveDrag = this.#moveDrag.bind(this);
            this.boundEndDrag = this.#endDrag.bind(this);
            sparePieces.querySelectorAll("img").forEach((img) => {
                img.addEventListener("mousedown", this.boundStartDrag);
                img.addEventListener("touchstart", this.boundStartDrag);
            });
            document.addEventListener("mousemove", this.boundMoveDrag);
            document.addEventListener("touchmove", this.boundMoveDrag, {
                passive: false,
            });
            document.addEventListener("mouseup", this.boundEndDrag);
            document.addEventListener("touchend", this.boundEndDrag);
            document.addEventListener("dragstart", (e) => {
                if (e.target.classList.contains("chessPiece")) {
                    e.preventDefault();
                }
            });
            this.#board.appendChild(sparePieces);
        } else {
            const sparePieces = this.#board.querySelector(".sparePieces");
            if (sparePieces) {
                sparePieces.remove();
            }
        }
    }
    #clearBoard() {
        const pieces = this.#chessboard.querySelectorAll(".chessPiece");
        pieces.forEach((piece) => {
            piece.classList.add("fade-out");
            piece.addEventListener(
                "animationend",
                () => {
                    piece.remove();
                },
                { once: true }
            );
        });
    }
    #setPosition(fen) {
        let flag = 0;
        if (this.#chessboard.children[0].id != "sq1") {
            this.flipBoard();
            flag = 1;
        }
        this.#clearBoard();
        this.#fenToBoard(fen)
            .flat()
            .forEach((cell, idx) => {
                if (cell)
                    this.#chessboard.children[idx].appendChild(
                        this.#createImage(this.#fenChar[cell])
                    );
            });
        if (flag) this.flipBoard();
    }
    start() {
        this.#enPassantSquare = "";
        this.#castlingRights = { w: ["K", "Q"], b: ["k", "q"] };
        this.turn = "w";
        this.#halfMoveCount = 0;
        this.#fullMoveCount = 1;
        this.#setPosition(
            "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
        );
    }
    flipBoard() {
        if (this.showNotation) {
            this.#disableCoordinates();
            this.showNotation = true;
        }
        const rows = Array.from(this.#chessboard.children);
        rows.reverse();
        rows.forEach((row) => this.#chessboard.appendChild(row));
        if (this.showNotation) {
            this.#enableCoordinates();
        }
    }
    #fenToBoard(fen) {
        return fen
            .split(" ")[0]
            .split("/")
            .map((row) => {
                return Array.from(row, (char) => {
                    return isNaN(char) ? char : Array(parseInt(char)).fill("");
                }).flat();
            });
    }

    #repetitionCheck = {
    }


    #checkCheck(kingPos, fen, turn) {
        const board = this.#fenToBoard(fen).flat();
        const queen = turn === "w" ? "q" : "Q";
        const bishop = turn === "w" ? "b" : "B";
        const rook = turn === "w" ? "r" : "R";
        const pawn = turn === "w" ? "p" : "P";
        const direction = turn === "w" ? -1 : 1;
        const sense = (piece) =>
            piece && piece.toUpperCase
                ? piece === piece.toUpperCase()
                    ? "w"
                    : "b"
                : null;
        let i = kingPos;
        if (board[kingPos] === "") {
            i = board.indexOf(turn === "b" ? "k" : "K");
            kingPos = i;
        }
        while (
            i + 8 * direction > -1 &&
            i + 8 * direction < 64 &&
            i > -1 &&
            i < 64
        ) {
            i += direction * 8;
            if ([rook, queen].includes(board[i])) return true;
            if (!board[i]) continue;
            if (
                ["P", "N", "B"].includes(board[i].toUpperCase()) ||
                (board[i] && sense(board[i])) === turn
            )
                break;
        }
        i = kingPos;
        while (
            i - 8 * direction > -1 &&
            i - 8 * direction < 64 &&
            i > -1 &&
            i < 64
        ) {
            i -= direction * 8;
            if ([rook, queen].includes(board[i])) return true;
            if (!board[i]) continue;
            if (
                ["P", "N", "B"].includes(board[i].toUpperCase()) ||
                (board[i] && sense(board[i])) === turn
            )
                break;
        }
        i = kingPos;
        while ((i + 1) % 8 != 0 && i + 1 < 64 && i > -1 && i < 64) {
            i += 1;
            if ([rook, queen].includes(board[i])) return true;
            if (!board[i]) continue;
            if (
                ["P", "N", "B"].includes(board[i].toUpperCase()) ||
                (board[i] && sense(board[i])) === turn
            )
                break;
        }
        i = kingPos;
        while (i % 8 != 0 && i - 1 > -1 && i > -1 && i < 64) {
            i -= 1;
            if ([rook, queen].includes(board[i])) return true;
            if (!board[i]) continue;
            if (
                ["P", "N", "B"].includes(board[i].toUpperCase()) ||
                (board[i] && sense(board[i])) === turn
            )
                break;
        }
        i = kingPos;
        while (
            ((i % 8 !== 0 && turn === "w") || ((i + 1) % 8 !== 0 && turn === "b")) &&
            i > -1 &&
            i < 64
        ) {
            i += 9 * direction;
            if (
                (i > -1 && i < 64 && [queen, bishop].includes(board[i])) ||
                (board[i] === pawn && i === kingPos + 9 * direction)
            )
                return true;
            if (!board[i]) continue;
            if (
                ["N", "R"].includes(board[i].toUpperCase()) ||
                (board[i] && sense(board[i])) === turn
            )
                break;
        }
        i = kingPos;
        while (
            ((i % 8 !== 0 && turn === "b") || ((i + 1) % 8 !== 0 && turn === "w")) &&
            i > -1 &&
            i < 64
        ) {
            i += 7 * direction;
            if (
                (i > -1 && i < 64 && [queen, bishop].includes(board[i])) ||
                (board[i] === pawn && i === kingPos + 9 * direction)
            )
                return true;
            if (!board[i]) continue;
            if (
                ["N", "R"].includes(board[i].toUpperCase()) ||
                (board[i] && sense(board[i])) === turn
            )
                break;
        }
        i = kingPos;
        while (
            ((i % 8 !== 0 && turn === "b") || ((i + 1) % 8 !== 0 && turn === "w")) &&
            i > -1 &&
            i < 64
        ) {
            i -= 9 * direction;
            if (i > -1 && i < 64 && [queen, bishop].includes(board[i])) return true;
            if (!board[i]) continue;
            if (
                ["N", "R"].includes(board[i].toUpperCase()) ||
                (board[i] && sense(board[i])) === turn
            )
                break;
        }
        i = kingPos;
        while (
            ((i % 8 !== 0 && turn === "w") || ((i + 1) % 8 !== 0 && turn === "b")) &&
            i > -1 &&
            i < 64
        ) {
            i -= 7 * direction;
            if (i > -1 && i < 64 && [queen, bishop].includes(board[i])) return true;
            if (!board[i]) continue;
            if (
                ["N", "R"].includes(board[i].toUpperCase()) ||
                (board[i] && sense(board[i])) === turn
            )
                break;
        }
        i = kingPos;
        if ((i + 9 * direction) < 64 && (i + 9 * direction > -1) && i % 8 !== 0 && board[i + 9] === (turn === "w" ? "p" : "P")) return true;
        if ((i + 7 * direction) < 64 && (i + 7 * direction > -1) && (i + 1) % 8 !== 0 && board[i + 7] === (turn === "w" ? "p" : "P")) return true;
        i = kingPos;
        const kingAttack = [1, -1, 7, -7, 8, -8, 9, -9];
        for (const attack of kingAttack) {
            let idx = kingPos + attack;
            if (([-9, 7, -1].includes(attack) && kingPos % 8 !== 0 && board[idx] && board[idx] === (turn === "w" ? "k" : "K"))) return true;
            if (([-7, 9, 1].includes(attack) && (kingPos + 1) % 8 !== 0) && board[idx] && board[idx] === (turn === "w" ? "k" : "K")) return true;
            if (idx > -1 && idx < 64 && board[idx] && board[idx] === (turn === "w" ? "k" : "K")) return true;
        }
        const kg = "sq" + (kingPos + 1);
        const knightCheck = this.validKnightMoves(kg);
        for (const knight of knightCheck) {
            let idx = parseInt(knight.match(/\d+/)[0]) - 1;
            if (board[idx] === (turn === "w" ? "n" : "N")) return true;
        }
        return false;
    }
    #isCheck() {
        const fen = this.fen();
        const board = this.#fenToBoard(fen).flat();
        const turn = this.turn;
        const king = board.indexOf(turn === "w" ? "K" : "k");
        return this.#checkCheck(king, fen, turn);
    }

    #isCheckMate() {
        const fen = this.fen();
        const board = this.#fenToBoard(fen).flat();
        const turn = this.turn;
        const allMoves = this.#allMoves(turn, fen);
        if (allMoves.length === 0 && this.#isCheck()) {
            this.#playSound("checkmate");
            this.status = "checkmate";
            const changeTurn = this.#nextTurn();
            const modal = this.#board.querySelector("#myModal");
            const message = changeTurn === "b" ? "Black is won by Checkmate!" : "White is won by Checkmate!";
            modal.querySelector(".modal-content").querySelector('p').innerText = message;
            setTimeout(() => {
                this.disableDrag();
                this.#stopTimer();
                modal.style.display = "block";
            }, 250);
        }
    }
    #checkThreefoldRepetition() {
        let fen = this.fen().split(" ")[0];
        if (this.#repetitionCheck[fen]) {
            this.#repetitionCheck[fen] += 1
        } else {
            this.#repetitionCheck[fen] = 1
        }
        if (this.#repetitionCheck[fen] === 3) {
            this.status = "draw";
            this.#playSound("checkmate");
            const changeTurn = this.#nextTurn();
            const modal = this.#board.querySelector("#myModal");
            const message = "Draw by Threefold Repetition!";
            modal.querySelector(".modal-content").querySelector('p').innerText = message;
            setTimeout(() => {
                this.disableDrag();
                this.#stopTimer();
                modal.style.display = "block";
            }, 250);
        }
    }
    #checkFiftyMoveRule() {
        if (this.#halfMoveCount >= 100) {
            this.#playSound("checkmate");
            this.status = "DrawByFiftyMoveRule";
            const changeTurn = this.#nextTurn();
            const modal = this.#board.querySelector("#myModal");
            const message = "Draw by Fifty Move Rule!";
            modal.querySelector(".modal-content").querySelector('p').innerText = message;
            setTimeout(() => {
                this.disableDrag();
                this.#stopTimer();
                modal.style.display = "block";
            }, 250);
        }
    }

    #checkStaleMate() {
        const fen = this.fen();
        const allMoves = this.#allMoves(this.turn, fen);
        if (allMoves.length === 0 && !this.#isCheck()) {
            this.#playSound("checkmate");
            this.status = "stalemate";
            const changeTurn = this.#nextTurn();
            const modal = this.#board.querySelector("#myModal");
            const message = "Draw by Stalemate!";
            modal.querySelector(".modal-content").querySelector('p').innerText = message;
            setTimeout(() => {
                this.disableDrag();
                this.#stopTimer();
                modal.style.display = "block";
            }, 250);
        }
    }

    #checkDraw() {
        const board = this.#fenToBoard(this.fen()).flat();
        const pieces = board.filter((piece) => piece);
        let status = '';

        const hasQueenOrRook = pieces.includes('q') || pieces.includes('Q') || pieces.includes('r') || pieces.includes('R');
        const hasPawn = pieces.includes('p') || pieces.includes('P');

        // Only kings
        if (pieces.length <= 2) {
            status = "DrawByInsufficientMaterial";
        }
        // King and one minor piece
        else if (pieces.length === 3 && !hasQueenOrRook && !hasPawn) {
            status = "DrawByInsufficientMaterial";
        }
        // Two bishops of the same color
        else if (pieces.length === 4 && !hasQueenOrRook && !hasPawn) {
            const bishops = pieces.filter(piece => piece.toLowerCase() === 'b');
            const knights = pieces.filter(piece => piece.toLowerCase() === 'n');

            if (bishops.length === 2 || knights.length === 2 || (bishops.length === 1 && knights.length === 1)) {
                status = "DrawByInsufficientMaterial";
            }
        }

        if (status) {
            this.status = status;
            this.#playSound("checkmate");
            const changeTurn = this.#nextTurn();
            const modal = this.#board.querySelector("#myModal");
            const message = "Draw by Insufficient Material!";
            modal.querySelector(".modal-content").querySelector('p').innerText = message;
            setTimeout(() => {
                this.disableDrag();
                this.#stopTimer();
                modal.style.display = "block";
            }, 250);
        }
    }


    #checkStatus() {
        this.#isCheckMate();
        this.#checkFiftyMoveRule();
        this.#checkThreefoldRepetition();
        this.#checkStaleMate();
        this.#checkDraw();
    }

    fen() {
        const squares = this.#chessboard.querySelectorAll(".square");
        let fen = "";
        let empty = 0;
        for (let i = 0; i < 64; i++) {
            if (i % 8 === 0 && i !== 0) {
                if (empty) {
                    fen += empty;
                    empty = 0;
                }
                fen += "/";
            }
            if (squares[i].querySelector(".chessPiece")) {
                if (empty) {
                    fen += empty;
                    empty = 0;
                }
                fen +=
                    this.#_fenChar[
                    this.#_pieceMap[
                    squares[i].querySelector(".chessPiece").classList[0]
                    ]
                    ];
            } else {
                empty++;
            }
        }
        if (empty) fen += empty;
        if (this.#chessboard.children[0].id !== "sq1")
            fen = fen.split("").reverse().join("");
        fen += " " + this.turn + " ";
        let castle =
            (this.#castlingRights["w"][0] || "") +
            (this.#castlingRights["w"][1] || "") +
            (this.#castlingRights["b"][0] || "") +
            (this.#castlingRights["b"][1] || "");
        fen += castle || "-";
        fen +=
            " " +
            (this.#enPassantSquare
                ? this.convertIdtoCoord(this.#enPassantSquare)
                : "-") +
            " " +
            this.#halfMoveCount +
            " " +
            this.#fullMoveCount;
        return fen;
    }
    #toggleHighlightSquare() {
        this.#highlightSquare =
            this.#highlightSquare === "hovered" ? "unhovered" : "hovered";
    }
    #toggleBlindFold() {
        Array.from(this.#chessboard.getElementsByClassName("chessPiece")).forEach(
            (image) => {
                image.style.visibility =
                    image.style.visibility === "hidden" ? "visible" : "hidden";
            }
        );
    }
    convertIdtoCoord(str) {
        const num = parseInt(str.match(/\d+/)[0]);
        const files = ["h", "a", "b", "c", "d", "e", "f", "g"];
        return files[num % 8] + (8 - Math.floor((num - 1) / 8));
    }
    convertCoordtoId(str) {
        const obj = { a: 0, b: 1, c: 2, d: 3, e: 4, f: 5, g: 6, h: 7 };
        return "sq" + (8 * (8 - parseInt(str[1])) + obj[str[0]] + 1);
    }
    #showPrompt(sqr, orientation) {
        return new Promise((resolve) => {
            const prompt = document.createElement("div");
            prompt.classList.add("prompt");
            prompt.style = `display:none;position:absolute;background-color:white;width:100%;aspect-ratio:1/4;box-shadow:0 0 10px rgba(0,0,0,0.5);text-align:center;z-index:200;top:0;`;
            if (
                (orientation === "black" && this.turn === "w") ||
                (orientation === "white" && this.turn === "b")
            ) {
                prompt.style.top = "";
                prompt.style.bottom = 0;
            }
            const pieceTypes = ["q", "n", "b", "r"];
            pieceTypes.forEach((piece) => {
                const pieceDiv = document.createElement("div");
                pieceDiv.classList.add("images");
                pieceDiv.style = `background-image:url(media/chess-pieces/${this.#pieceTheme}/${this.turn}${piece}.${this.#format});background-size:contain;aspect-ratio:1/1;width:100%;background-color:white;cursor:pointer;`;
                pieceDiv.addEventListener("mouseover", () => {
                    pieceDiv.style.backgroundColor = "#FFDFD6";
                });
                pieceDiv.addEventListener("mouseout", () => {
                    pieceDiv.style.backgroundColor = "white";
                });
                pieceDiv.addEventListener("mousedown", () => {
                    this.#promotingPiece = piece;
                    resolve(piece);
                });
                prompt.appendChild(pieceDiv);
            });
            this.#chessboard.querySelector(`#${sqr}`).appendChild(prompt);
            prompt.style.display = "block";
        });
    }
    #hidePrompt() {
        if (this.#chessboard.querySelector(".prompt"))
            this.#chessboard.querySelector(".prompt").remove();
    }
    #chessMap = {
        bk: "\u265A", // Black King to White King
        bq: "\u265B", // Black Queen to White Queen
        br: "\u265C", // Black Rook to White Rook
        bb: "\u265D", // Black Bishop to White Bishop
        bn: "\u265E", // Black Knight to White Knight
        bp: "",       // Black Pawn remains unchanged (assuming empty string is intentional)
        wk: "\u2654", // White King to Black King
        wq: "\u2655", // White Queen to Black Queen
        wr: "\u2656", // White Rook to Black Rook
        wb: "\u2657", // White Bishop to Black Bishop
        wn: "\u2658", // White Knight to Black Knight
        wp: "",       // White Pawn remains unchanged (assuming empty string is intentional)
    };
    moveType = { check: "+", checkmate: "#", capture: "x", move: "" };
    checkCastle(start, end) {
        let board = this.#fenToBoard(this.fen()).flat();
        let king = this.turn === "w" ? "K" : "k";
        if (
            king === 'k' &&
            this.#castlingRights[this.turn][1] &&
            start === "sq5" &&
            end === "sq3")
            return true;
        if (
            king === 'k' &&
            this.#castlingRights[this.turn][0] &&
            start === "sq5" &&
            end === "sq7")
            return true;
        if (
            king === 'K' &&
            this.#castlingRights[this.turn][1] &&
            start === "sq61" &&
            end === "sq59"
        )
            return true;
        if (
            king === 'K' &&
            this.#castlingRights[this.turn][0] &&
            start === "sq61" &&
            end === "sq63"
        )
            return true;
        return false;
    }
    canEnPassant(start, end) {
        let board = this.#fenToBoard(this.fen()).flat();
        let turn = this.turn;
        let s = parseInt(start.match(/\d+/)[0]) - 1;
        let e = parseInt(end.match(/\d+/)[0]) - 1;
        let l = e + 1;
        let r = e - 1;
        let p = turn === "w" ? "P" : "p";
        if (
            board[e].toUpperCase() === "P" &&
            Math.abs(s - e) === 16 &&
            (board[l] === p || board[r] === p)
        ) {
            this.#enPassantSquare = "sq" + (s + (turn === "b" ? -8 : 8) + 1);
        } else {
            this.#enPassantSquare = "";
        }
        console.log(this.#enPassantSquare);
    }
    async makeMove(start, end, method) {
        if (!start || !end) return;
        if (this.checkCastle(start, end)) {
            this.castlingMove(end, method);
            return;
        }
        const startSquare = this.#chessboard.querySelector(`#${start}`);
        const endSquare = this.#chessboard.querySelector(`#${end}`);
        const image =
            this.#draggableDiv || startSquare.querySelector(".chessPiece");
        const endImage = endSquare.querySelector(".chessPiece");
        if (
            image &&
            image.classList[0].includes("Pawn") &&
            ((end.match(/\d+/)[0] <= 64 && end.match(/\d+/)[0] >= 57) ||
                (end.match(/\d+/)[0] >= 1 && end.match(/\d+/)[0] <= 8))
        ) {
            startSquare.appendChild(image);
            this.fixImage(image);
            let toPromote = null;
            let newImage = null;
            if (method.includes('engineSays')) {
                toPromote = method.split('_')[1];
                newImage = this.#createImage(`${this.turn}${toPromote}`);
            }
            else {
                toPromote = await this.#showPrompt(end, this.orientation);
                newImage = this.#createImage(`${this.turn}${toPromote}`);
            }

            this.#hideLegalMoves();
            let takes = "";
            if (endImage) {
                endImage.remove();
                takes = this.convertIdtoCoord(start)[0] + "x";
            }
            this.fixImage(newImage);
            startSquare.removeChild(image);
            image.remove();
            endSquare.appendChild(newImage);
            if (this.turn === "b") this.#fullMoveCount++;
            this.#halfMoveCount = 0;
            takes += this.convertIdtoCoord(end);
            takes += "=" + this.#chessMap[`${this.turn}${toPromote}`];
            this.#changeTurn();
            this.#FEN = this.fen();
            const move = {
                from: { sqr: start, piece: image.cloneNode(true) },
                to: { sqr: end, piece: endImage ? endImage.cloneNode(true) : null },
                promotedTo: newImage.cloneNode(true),
                sound: "",
            };
            if (this.#isCheck()) {
                move.sound = "check";
            } else if (endImage) {
                move.sound = "capture";
            } else {
                move.sound = "promotion";
            }
            if (move.sound === "check") takes += this.moveType[move.sound];
            this.PGN.push(takes);
            this.#playSound(move.sound);
            this.#moveStack.push(move);
            this.#draggableDiv = null;
            this.#originalParent = null;
            this.#startSquare = "";
            this.#endSquare = "";
            this.#FEN = this.fen();
            this.#checkStatus();
            if (this.#BRIDGE) {
                this.#BRIDGE();
            }
            return;
        }
        if (method === "byDrag") {
            this.#hideLegalMoves();
            const endImageFlag = endImage ? 1 : 0;
            const flag = this.#enPassantSquare;
            const move = {
                from: { sqr: start, piece: image.cloneNode(true) },
                to: { sqr: end, piece: endImage ? endImage.cloneNode(true) : null },
                promotedTo: null,
                sound: "",
            };
            if (endImage) {
                endImage.remove();
                this.#halfMoveCount = 0;
            } else if (image.classList[0].includes("Pawn")) {
                this.#halfMoveCount = 0;
            } else {
                this.#halfMoveCount++;
            }
            this.fixImage(image);
            endSquare.appendChild(image);
            this.#FEN = this.fen();
            if (this.turn === "b") this.#fullMoveCount++;
            this.#changeTurn();
            if (this.#isCheck()) {
                move.sound = "check";
            } else if (endImageFlag) {
                move.sound = "capture";
            } else if (flag === end) {
                move.sound = "capture";
                let sq =
                    "sq" +
                    (parseInt(flag.match(/\d+/)[0]) + (this.turn === "w" ? -8 : 8));
                let en = this.#chessboard
                    .querySelector(`#${sq}`)
                    .querySelector(".chessPiece");
                let enPasant = { sqr: sq, piece: en.cloneNode(true) };
                move.enPassant = enPasant;
                en.remove();
            } else {
                move.sound = "move";
            }
            this.#playSound(move.sound);
            this.canEnPassant(start, end);
            let pcs = this.#chessMap[image.classList[2]];
            if (move.sound === "capture" && image.classList[0].includes("Pawn")) {
                pcs += this.convertIdtoCoord(start)[0] + this.moveType[move.sound];
            } else if (move.sound === "capture") pcs += this.moveType[move.sound];
            pcs += this.convertIdtoCoord(end);
            if (move.sound === "check") pcs += this.moveType[move.sound];
            this.PGN.push(pcs);
            this.#moveStack.push(move);
            this.#draggableDiv = null;
            this.#originalParent = null;
            this.#startSquare = "";
            this.#endSquare = "";
            if (image.classList[2][1] === "k") {
                this.#castlingRights[image.classList[2][0]][0] = "";
                this.#castlingRights[image.classList[2][0]][1] = "";
            } else if (
                (image.classList[2][1] === "r" && start === "sq1") ||
                start === "sq57"
            ) {
                this.#castlingRights[image.classList[2][0]][1] = "";
            } else if (
                (image.classList[2][1] === "r" && start === "sq8") ||
                start === "sq64"
            ) {
                this.#castlingRights[image.classList[2][0]][0] = "";
            }
            this.#FEN = this.fen();
            if (this.#BRIDGE) {
                this.#BRIDGE();
            }
            this.#checkStatus();
            return;
        } else if (method === "byClick") {
            const startRect = startSquare.getBoundingClientRect();
            const endRect = endSquare.getBoundingClientRect();
            const deltaX = endRect.left - startRect.left;
            const deltaY = endRect.top - startRect.top;
            image.style.transition = "transform 0.2s ease";
            image.style.transform = `translate(${deltaX}px,${deltaY}px)`;
            this.#hideLegalMoves();
            setTimeout(() => {
                image.style.transform = "";
                const endImageFlag = endImage ? 1 : 0;
                const flag = this.#enPassantSquare;
                const move = {
                    from: { sqr: start, piece: image.cloneNode(true) },
                    to: { sqr: end, piece: endImage ? endImage.cloneNode(true) : null },
                    promotedTo: null,
                    sound: "",
                };
                if (endImage) {
                    endImage.remove();
                    this.#halfMoveCount = 0;
                } else if (image.classList[0].includes("Pawn")) {
                    this.#halfMoveCount = 0;
                } else {
                    this.#halfMoveCount++;
                }
                this.fixImage(image);
                endSquare.appendChild(image);
                this.#FEN = this.fen();
                if (this.turn === "b") this.#fullMoveCount++;
                this.#changeTurn();
                if (this.#isCheck()) {
                    move.sound = "check";
                } else if (endImageFlag) {
                    move.sound = "capture";
                } else if (flag === end) {
                    move.sound = "capture";
                    let sq =
                        "sq" +
                        (parseInt(flag.match(/\d+/)[0]) + (this.turn === "w" ? -8 : 8));
                    let en = this.#chessboard
                        .querySelector(`#${sq}`)
                        .querySelector(".chessPiece");
                    let enPasant = { sqr: sq, piece: en.cloneNode(true) };
                    move.enPassant = enPasant;
                    en.remove();
                } else {
                    move.sound = "move";
                }
                this.#playSound(move.sound);
                this.canEnPassant(start, end);
                let pcs = this.#chessMap[image.classList[2]];
                if (move.sound === "capture" && image.classList[0].includes("Pawn")) {
                    pcs += this.convertIdtoCoord(start)[0] + this.moveType[move.sound];
                } else if (move.sound === "capture") pcs += this.moveType[move.sound];
                pcs += this.convertIdtoCoord(end);
                if (move.sound === "check") pcs += this.moveType[move.sound];
                this.PGN.push(pcs);
                this.#moveStack.push(move);
                this.#draggableDiv = null;
                this.#originalParent = null;
                this.#startSquare = "";
                this.#endSquare = "";
                if (image.classList[2][1] === "k") {
                    this.#castlingRights[image.classList[2][0]][0] = "";
                    this.#castlingRights[image.classList[2][0]][1] = "";
                } else if (
                    (image.classList[2][1] === "r" && start === "sq1") ||
                    start === "sq57"
                ) {
                    this.#castlingRights[image.classList[2][0]][1] = "";
                } else if (
                    (image.classList[2][1] === "r" && start === "sq8") ||
                    start === "sq64"
                ) {
                    this.#castlingRights[image.classList[2][0]][0] = "";
                }
                this.#FEN = this.fen();
                if (this.#BRIDGE) {
                    this.#BRIDGE();
                }
                this.#checkStatus();
            }, 200);
            return;
        }
    }
    castle = {
        sq7: ["sq6", "sq5", "sq8"],
        sq63: ["sq62", "sq61", "sq64"],
        sq3: ["sq4", "sq5", "sq1"],
        sq59: ["sq60", "sq61", "sq57"],
    };
    castleSymbol = { sq7: "O-O", sq63: "O-O", sq3: "O-O-O", sq59: "O-O-O" };
    castlingMove(sqr, method) {
        const s = this.#chessboard.querySelector(`#${this.castle[sqr][1]}`);
        const e = this.#chessboard.querySelector(`#${sqr}`);
        const k = this.#draggableDiv || s.querySelector(".chessPiece");
        const startRect = s.getBoundingClientRect();
        const endRect = e.getBoundingClientRect();
        const deltaX = endRect.left - startRect.left;
        const deltaY = endRect.top - startRect.top;
        if (method !== "byDrag") {
            k.style.transition = "transform 0.2s ease";
            k.style.transform = `translate(${deltaX}px,${deltaY}px)`;
        }
        setTimeout(() => {
            this.#playSound("castle");
            k.style.transform = "";
            e.appendChild(k);
            this.fixImage(k);
            this.#castlingRights[k.classList[2][0]][0] = "";
            this.#castlingRights[k.classList[2][0]][1] = "";
        }, 200);
        const sr = this.#chessboard.querySelector(`#${this.castle[sqr][2]}`);
        const er = this.#chessboard.querySelector(`#${this.castle[sqr][0]}`);
        const r = sr.querySelector(".chessPiece");
        const startRectR = sr.getBoundingClientRect();
        const endRectR = er.getBoundingClientRect();
        const deltaXR = endRectR.left - startRectR.left;
        const deltaYR = endRectR.top - startRectR.top;
        r.style.transition = "transform 0.2s ease";
        r.style.transform = `translate(${deltaXR}px,${deltaYR}px)`;
        setTimeout(() => {
            r.style.transform = "";
            er.appendChild(r);
            this.fixImage(r);
        }, 200);
        const moveR = {
            from: { sqr: this.castle[sqr][2], piece: r.cloneNode(true) },
            to: { sqr: this.castle[sqr][0], piece: null },
            promotedTo: null,
            sound: "",
        };
        const move = {
            from: { sqr: this.castle[sqr][1], piece: k.cloneNode(true) },
            to: { sqr: sqr, piece: null },
            promotedTo: null,
            sound: "castle",
            castle: moveR,
        };
        this.#moveStack.push(move);
        this.#hideLegalMoves();
        this.#draggableDiv = null;
        this.#originalParent = null;
        this.#startSquare = "";
        this.#endSquare = "";
        this.#halfMoveCount++;
        if (this.turn === "b") this.#fullMoveCount++;
        this.#FEN = this.fen();
        this.#changeTurn();
        this.PGN.push(this.castleSymbol[sqr]);
        if (this.#BRIDGE) {
            this.#BRIDGE();
        }
        return;
    }
    fixImage(img) {
        img.style.position = "relative";
        img.style.top = 0;
        img.style.left = 0;
    }
    undoMove() {
        if (!this.#undo) return;
        this.#hideLegalMoves();
        const move = this.#moveStack.pop();
        if (!move) return;
        let castle = move.castle ? move.castle : null;
        const startSquare = this.#chessboard.querySelector(`#${move.from.sqr}`);
        const endSquare = this.#chessboard.querySelector(`#${move.to.sqr}`);
        let image = move.from.piece;
        let endImage = move.to.piece;
        let promotedTo = move.promotedTo;
        if (promotedTo) {
            endSquare.querySelector(".chessPiece").replaceWith(image);
        }
        const startRect = startSquare.getBoundingClientRect();
        const endRect = endSquare.getBoundingClientRect();
        const deltaX = -endRect.left + startRect.left;
        const deltaY = -endRect.top + startRect.top;
        image = endSquare.querySelector(".chessPiece");
        image.style.transition = "transform 0.2s ease";
        image.style.transform = `translate(${deltaX}px,${deltaY}px)`;
        setTimeout(() => {
            startSquare.appendChild(image);
            this.fixImage(image);
            image.style.transform = "";
            this.#hideLegalMoves();
            if (endImage) {
                endSquare.appendChild(endImage);
                this.fixImage(endImage);
            }
            this.#changeTurn();
            this.#playSound(move.sound);
        }, 200);
        if (castle) {
            const startSquareR = this.#chessboard.querySelector(
                `#${castle.from.sqr}`
            );
            const endSquareR = this.#chessboard.querySelector(`#${castle.to.sqr}`);
            let imageR = castle.from.piece;
            let endImageR = castle.to.piece;
            const startRectR = startSquareR.getBoundingClientRect();
            const endRectR = endSquareR.getBoundingClientRect();
            const deltaXR = -endRectR.left + startRectR.left;
            const deltaYR = -endRectR.top + startRectR.top;
            imageR = endSquareR.querySelector(".chessPiece");
            imageR.style.transition = "transform 0.2s ease";
            imageR.style.transform = `translate(${deltaXR}px,${deltaYR}px)`;
            setTimeout(() => {
                startSquareR.appendChild(imageR);
                this.fixImage(imageR);
                imageR.style.transform = "";
                this.#playSound(castle.sound);
            }, 200);
        }
        if (move.enPassant) {
            let sq = move.enPassant.sqr;
            let en = move.enPassant.piece;
            this.#chessboard.querySelector(`#${sq}`).appendChild(en);
        }
    }
    validPawnMoves(squareId) {
        const fen = this.fen();
        const board = this.#fenToBoard(fen).flat();
        const KeyNumber = parseInt(squareId.match(/\d+/)[0]) - 1;
        const piece = board[KeyNumber];
        const moves = [];
        const row = Math.floor(KeyNumber / 8);
        const direction = piece === piece.toUpperCase() ? -1 : 1;
        const isStartingPosition =
            (direction === -1 && row === 6) || (direction === 1 && row === 1);
        const frontSquareId = KeyNumber + 8 * direction;
        const frontSquare = board[frontSquareId];
        if (frontSquareId >= 0 && frontSquareId < 64 && !frontSquare) {
            moves.push("sq" + (frontSquareId + 1));
            if (isStartingPosition) {
                const doubleFrontSquareId = KeyNumber + 16 * direction;
                const doubleFrontSquare = board[doubleFrontSquareId];
                if (
                    doubleFrontSquareId >= 0 &&
                    doubleFrontSquareId < 64 &&
                    !doubleFrontSquare
                ) {
                    moves.push("sq" + (doubleFrontSquareId + 1));
                }
            }
        }
        const rightSq = KeyNumber + 7 * direction;
        const leftSq = KeyNumber + 9 * direction;
        if (direction === -1) {
            if (
                KeyNumber % 8 !== 0 &&
                board[leftSq] &&
                board[leftSq].toLowerCase() === board[leftSq]
            ) {
                moves.push("sq" + (leftSq + 1));
            }
            if (KeyNumber % 8 !== 0 && this.#enPassantSquare === "sq" + (leftSq + 1)) {
                moves.push("sq" + (leftSq + 1));
            }
            if (
                (KeyNumber + 1) % 8 !== 0 &&
                board[rightSq] &&
                board[rightSq].toLowerCase() === board[rightSq]
            ) {
                moves.push("sq" + (rightSq + 1));
            }
            if (
                KeyNumber % 8 !== 0 &&
                this.#enPassantSquare === "sq" + (rightSq + 1)
            ) {
                moves.push("sq" + (rightSq + 1));
            }
        } else {
            if (
                KeyNumber % 8 !== 0 &&
                board[leftSq] &&
                board[leftSq].toUpperCase() === board[leftSq]
            ) {
                moves.push("sq" + (leftSq + 1));
            }
            if (KeyNumber % 8 !== 0 && this.#enPassantSquare === "sq" + (leftSq + 1)) {
                moves.push("sq" + (leftSq + 1));
            }
            if (
                (KeyNumber + 1) % 8 !== 0 &&
                board[rightSq] &&
                board[rightSq].toUpperCase() === board[rightSq]
            ) {
                moves.push("sq" + (rightSq + 1));
            }
            if (
                (KeyNumber + 1) % 8 !== 0 &&
                this.#enPassantSquare === "sq" + (rightSq + 1)
            ) {
                moves.push("sq" + (rightSq + 1));
            }
        }
        return moves;
    }
    validBishopMoves(squareId) {
        const fen = this.fen();
        const board = this.#fenToBoard(fen).flat();
        const KeyNumber = parseInt(squareId.match(/\d+/)[0]) - 1;
        const piece = board[KeyNumber];
        const moves = [];
        let isenemy = (pcs) =>
            (piece === piece.toUpperCase()) !== (pcs === pcs.toUpperCase());
        let i = KeyNumber;
        while ((i + 1) % 8 != 0 && i < 64) {
            i += 9;
            const sq = board[i];
            if (!sq && i < 64 && i >= 0) {
                moves.push("sq" + (i + 1));
            } else if (sq && isenemy(sq) && i < 64 && i >= 0) {
                moves.push("sq" + (i + 1));
                break;
            } else {
                break;
            }
        }
        i = KeyNumber;
        while (i % 8 != 0 && i >= 0) {
            i -= 9;
            const sq = board[i];
            if (!sq && i < 64 && i >= 0) {
                moves.push("sq" + (i + 1));
            } else if (sq && isenemy(sq) && i < 64 && i >= 0) {
                moves.push("sq" + (i + 1));
                break;
            } else {
                break;
            }
        }
        i = KeyNumber;
        while (i % 8 != 0 && i < 64 && i >= 0) {
            i += 7;
            if (i < 0) break;
            const sq = board[i];
            if (!sq && i < 64 && i >= 0) {
                moves.push("sq" + (i + 1));
            } else if (sq && isenemy(sq) && i < 64 && i >= 0) {
                moves.push("sq" + (i + 1));
                break;
            } else {
                break;
            }
        }
        i = KeyNumber;
        while ((i + 1) % 8 != 0 && i < 64) {
            i -= 7;
            if (i >= 64) break;
            const sq = board[i];
            if (!sq && i < 64 && i >= 0) {
                moves.push("sq" + (i + 1));
            } else if (sq && isenemy(sq) && i < 64 && i >= 0) {
                moves.push("sq" + (i + 1));
                break;
            } else {
                break;
            }
        }
        return moves;
    }
    validRookMoves(squareId) {
        const fen = this.fen();
        const board = this.#fenToBoard(fen).flat();
        const KeyNumber = parseInt(squareId.match(/\d+/)[0]) - 1;
        const piece = board[KeyNumber];
        const moves = [];
        let isenemy = (pcs) =>
            pcs && (piece === piece.toUpperCase()) !== (pcs === pcs.toUpperCase());
        let i = KeyNumber;
        while (i < 64) {
            i += 8;
            const sq = board[i];
            if (!sq && i < 64 && i >= 0) {
                moves.push("sq" + (i + 1));
            } else if (isenemy(sq) && i < 64 && i >= 0) {
                moves.push("sq" + (i + 1));
                break;
            } else {
                break;
            }
        }
        i = KeyNumber;
        while (i >= 8) {
            i -= 8;
            const sq = board[i];
            if (!sq && i < 64 && i >= 0) {
                moves.push("sq" + (i + 1));
            } else if (isenemy(sq) && i < 64 && i >= 0) {
                moves.push("sq" + (i + 1));
                break;
            } else {
                break;
            }
        }
        i = KeyNumber;
        while ((i + 1) % 8 != 0) {
            i += 1;
            const sq = board[i];
            if (!sq && i < 64 && i >= 0) {
                moves.push("sq" + (i + 1));
            } else if (isenemy(sq) && i < 64 && i >= 0) {
                moves.push("sq" + (i + 1));
                break;
            } else {
                break;
            }
        }
        i = KeyNumber;
        while (i % 8 != 0) {
            i -= 1;
            const sq = board[i];
            if (!sq && i < 64 && i >= 0) {
                moves.push("sq" + (i + 1));
            } else if (isenemy(sq) && i < 64 && i >= 0) {
                moves.push("sq" + (i + 1));
                break;
            } else {
                break;
            }
        }
        return moves;
    }
    validQueenMoves(squareId) {
        let moves = this.validRookMoves(squareId);
        moves = moves.concat(this.validBishopMoves(squareId));
        return moves;
    }
    validKnightMoves(squareId) {
        const fen = this.fen();
        const board = this.#fenToBoard(fen).flat();
        const KeyNumber = parseInt(squareId.match(/\d+/)[0]) - 1;
        const piece = board[KeyNumber];
        const moves = [];
        let isenemy = (pcs) =>
            pcs && (piece === piece.toUpperCase()) !== (pcs === pcs.toUpperCase());
        const offsets = [-17, -15, -10, -6, 6, 10, 15, 17];
        const validMoves = offsets
            .map((offset) => KeyNumber + offset)
            .filter((index) => {
                if (index < 0 || index >= 64) return false;
                let rowOriginal = Math.floor(KeyNumber / 8);
                let rowDestination = Math.floor(index / 8);
                let colOriginal = KeyNumber % 8;
                let colDestination = index % 8;
                if (
                    Math.abs(rowDestination - rowOriginal) > 2 ||
                    Math.abs(colDestination - colOriginal) > 2
                )
                    return false;
                return !board[index] || isenemy(board[index]);
            });
        validMoves.forEach((index) => moves.push("sq" + (index + 1)));
        return moves;
    }
    validKingMoves(squareId) {
        const fen = this.fen();
        const board = this.#fenToBoard(fen).flat();
        const keyNumber = parseInt(squareId.match(/\d+/)[0]) - 1;
        const piece = board[keyNumber];
        const moves = [];
        const potentialMoves = [
            keyNumber + 1,
            keyNumber - 1,
            keyNumber + 8,
            keyNumber - 8,
            keyNumber + 9,
            keyNumber - 9,
            keyNumber + 7,
            keyNumber - 7,
        ];
        const isEnemy = (pcs) =>
            pcs && (piece === piece.toUpperCase()) !== (pcs === pcs.toUpperCase());
        potentialMoves.forEach((move) => {
            if (move < 64 && move >= 0) {
                const targetPiece = board[move];
                if ([9, -7, 1].includes(move - keyNumber) && (keyNumber + 1) % 8 !== 0) {
                    if (!targetPiece || isEnemy(targetPiece)) {
                        moves.push(`sq${move + 1}`);
                    }
                }
                else if ([7, -9, -1].includes(move - keyNumber) && keyNumber % 8 !== 0) {
                    if (!targetPiece || isEnemy(targetPiece)) {
                        moves.push(`sq${move + 1}`);
                    }
                }
                else if (Math.abs(move - keyNumber) === 8) {
                    if (!targetPiece || isEnemy(targetPiece)) {
                        moves.push(`sq${move + 1}`);
                    }
                }
            }
        });
        const kingPos = keyNumber;
        const turn = this.turn;
        const sq1 = keyNumber + 1,
            sq2 = keyNumber + 2,
            sq3 = keyNumber - 1,
            sq4 = keyNumber - 2,
            sq5 = keyNumber - 3;
        if (
            !this.#checkCheck(kingPos, fen, turn) &&
            this.#castlingRights[this.turn][0] &&
            !board[sq1] &&
            !board[sq2] &&
            !this.#checkCheck(sq1, fen, turn) &&
            !this.#checkCheck(sq2, fen, turn)
        ) {
            moves.push(`sq${sq2 + 1}`);
        }
        if (
            !this.#checkCheck(kingPos, fen, turn) &&
            this.#castlingRights[this.turn][1] &&
            !board[sq3] &&
            !board[sq4] &&
            !board[sq5] &&
            !this.#checkCheck(sq3, fen, turn) &&
            !this.#checkCheck(sq4, fen, turn) &&
            !this.#checkCheck(sq5, fen, turn)
        ) {
            moves.push(`sq${sq4 + 1}`);
        }
        return moves;
    }
    #validMoves(squareId) {
        const square = this.#chessboard.querySelector(`#${squareId}`);
        const piece = square.querySelector(".chessPiece");
        if (!piece) {
            return [];
        } else if (piece.classList[0].includes("Pawn")) {
            let moves = this.validPawnMoves(squareId);
            return this.#filterMoves(moves, this.fen(), this.turn, squareId);
        } else if (piece.classList[0].includes("Rook")) {
            let moves = this.validRookMoves(squareId);
            return this.#filterMoves(moves, this.fen(), this.turn, squareId);
        } else if (piece.classList[0].includes("Bishop")) {
            let moves = this.validBishopMoves(squareId);
            return this.#filterMoves(moves, this.fen(), this.turn, squareId);
        } else if (piece.classList[0].includes("Queen")) {
            let moves = this.validQueenMoves(squareId);
            return this.#filterMoves(moves, this.fen(), this.turn, squareId);
        } else if (piece.classList[0].includes("Knight")) {
            let moves = this.validKnightMoves(squareId);
            return this.#filterMoves(moves, this.fen(), this.turn, squareId);
        } else if (piece.classList[0].includes("King")) {
            let moves = this.validKingMoves(squareId);
            return this.#filterMoves(moves, this.fen(), this.turn, squareId);
        }
    }
    #allMoves(turn, fen) {
        const bd = this.#fenToBoard(fen).flat();
        const board = bd.filter((piece) => piece);
        const moves = [];
        for (let i = 0; i < 64; i++) {
            const piece = board[i];
            const j = bd.indexOf(piece);
            if (piece && (piece === piece.toUpperCase()) === (turn === "w")) {
                const squareId = "sq" + (1 + j);
                const validMoves = this.#validMoves(squareId);
                validMoves.forEach((move) => moves.push(move));
            }
        }
        return moves;
    }
    #boardToFen(board) {
        let fen = "";
        let emptyCount = 0;
        for (let i = 0; i < board.length; i++) {
            const piece = board[i];
            if (piece === "") {
                emptyCount++;
            } else {
                if (emptyCount > 0) {
                    fen += emptyCount;
                    emptyCount = 0;
                }
                fen += piece;
            }
            if ((i + 1) % 8 === 0) {
                if (emptyCount > 0) {
                    fen += emptyCount;
                    emptyCount = 0;
                }
                if (i !== board.length - 1) {
                    fen += "/";
                }
            }
        }
        return fen;
    }
    #filterMoves(moves, fen, turn, startSq) {
        const chessBoard = this.#fenToBoard(fen).flat();
        const start = parseInt(startSq.match(/\d+/)[0]) - 1;
        const kingPos = chessBoard.indexOf(turn === "w" ? "K" : "k");
        const piece = chessBoard[start];
        chessBoard[start] = "";
        let filteredMoves = [];
        for (let i = 0; i < moves.length; i++) {
            let board = [...chessBoard];
            const move = moves[i];
            const targetSquare = parseInt(move.match(/\d+/)[0]) - 1;
            board[targetSquare] = piece;
            if (!this.#checkCheck(kingPos, this.#boardToFen(board), turn)) {
                filteredMoves.push(move);
            }
        }
        return filteredMoves;
    }
    allLegalMoves() {
        return this.#allMoves(this.turn, this.fen());
    }
}
