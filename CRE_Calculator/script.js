document.querySelector('select[name = "Question"]').addEventListener('change', function () {
    let question = this.value;
    if (question === 'xtov') {
        document.querySelector('input[name = "Conversion"]').placeholder = 'Conversion';
    } else {
        document.querySelector('input[name = "Conversion"]').placeholder = 'Volume in Litres';

    } 
});
document.querySelector('select[name = "Reactor"]').addEventListener('change', function () {
    let reactor = this.value;
    if (reactor === 'CSTR' || reactor === 'PFR') {
        document.querySelector('input[name = "VolumetricFlowRate"]').placeholder = '';
    } else {
        document.querySelector('input[name = "VolumetricFlowRate"]').placeholder = 'Volumetric Flow Rate in Litres/sec';
    }
});
document.querySelector('#reactionConditions').addEventListener('submit', function (event) {
    event.preventDefault(); // Prevent the form from being submitted to the server
    document.querySelector('#message').innerHTML = '';
    // debugger
    concentrationCalculator();
});
let DataA = [], DataB = [], DataC = [], DataD = [], DataRa = [], DataRb = [], DataRc = [], DataRd = [], DataFx = [];
function concentrationCalculator() {
    // debugger;
    let cia = Number(document.querySelector('input[name="ConcentrationA"]').value);
    let cib = Number(document.querySelector('input[name="ConcentrationB"]').value);
    let cic = Number(document.querySelector('input[name="ConcentrationC"]').value);
    let cid = Number(document.querySelector('input[name="ConcentrationD"]').value);
    let cii = Number(document.querySelector('input[name="Inert"]').value);
    let X = Number(document.querySelector('input[name="Conversion"]').value);
    let reactor = document.querySelector('select[name="Reactor"]').value;
    let phase = document.querySelector('select[name="Phase"]').value;
    let flowRate = Number(document.querySelector('input[name="VolumetricFlowRate"]').value);
    // let volume = document.querySelector('select[name="Volume"]').value;
    let pressure = document.querySelector('select[name="Pressure"]').value;
    let T0 = Number(document.querySelector('input[name="InitialT"]').value) + 273.15;
    let P0 = Number(document.querySelector('input[name="InitialP"]').value);
    // let T = Number(document.querySelector('input[name="FinalT"]').value);
    // let P = Number(document.querySelector('input[name="FinalP"]').value);
    let stoichiometry = document.querySelector('input[name="Stoichiometry"]').value;
    let [a, b, c, d] = stoichiometry.split(",").map(expression => eval(expression.trim()));
    console.log(cia, cib, cic, cid, a, b, c, d, phase);
    //for constant volume
    let delta = (d + c - a - b) / a, eps = 0, lr;
    if (phase === 'gas') { eps = delta * cia / (cia + cib + cic + cid +cii) };
    let maxConv = 1;
    if (cia / a <= cib / b) {
        lr = 'A';
        maxConv = 1;
    }
    else {
        lr = 'B';
        maxConv = cib * a / b;
        maxConv = maxConv / cia;
    }
    if (maxConv !== 1) {
        let message = "-> The limiting reactant is " + lr + " and the maximum conversion is " + maxConv+`.\n-> Epsilon is ${eps} and delta is ${delta}.`;
        document.querySelector('#message').innerHTML = message
    }
    else {
        let message = "-> The limiting reactant is " + lr + " and the maximum conversion is 1."+`.\n-> Epsilon is ${eps} and delta is ${delta}.`;
        document.querySelector('#message').innerHTML = message
    }
    let pf = (pressure === 'constantP' || phase === "liquid" || reactor === 'Batch') ? 0 : 1;
    let order = document.querySelector('input[name="Order"]').value;
    let k = Number(document.querySelector('input[name="RateConstant"]').value);
    let [p, q] = order.split(",").map(expression => eval(expression.trim()));
    let Px = (g) => { return P0 + cia * 0.08214 * T0 * pf * g * ((c + d - a - b) / a) }
    let ConcA = (x) => { return cia * (1 - x) * Px(x) / ((1 + eps * x) * (P0)) };
    let ConcB = (x) => { return cia * (cib / cia - x * b / a) * Px(x) / ((1 + eps * x) * (P0)) };
    let ConcC = (x) => { return cia * (cic / cia + x * c / a) * Px(x) / ((1 + eps * x) * (P0)) };
    let ConcD = (x) => { return cia * (cid / cia + x * d / a) * Px(x) / ((1 + eps * x) * (P0)) };
    DataA = [], DataB = [], DataC = [], DataD = [], DataRa = [], DataRb = [], DataRc = [], DataRd = [], DataFx = [];
    if (X > maxConv) X = maxConv;
    let step = X / 50;
    for (let x = 0; x <= X; x += step) {
        DataA.push({ x: x, y: ConcA(x) });
        DataB.push({ x: x, y: ConcB(x) });
        DataC.push({ x: x, y: ConcC(x) });
        DataD.push({ x: x, y: ConcD(x) });
        let r = k * (ConcA(x) ** p) * (ConcB(x) ** q);
        DataRa.push({ x: x, y: r });
        DataRb.push({ x: x, y: r * b / a });
        DataRc.push({ x: x, y: r * c / a });
        DataRd.push({ x: x, y: r * d / a });
        DataFx.push({ x: x, y: flowRate / r });
    }

    graphCalculator('plot1', DataA, "green", "Concentration of A", "Conversion", "Concentration");
    graphCalculator('plot2', DataB, "red", "Concentration of B", "Conversion", "Concentration");
    graphCalculator('plot3', DataC, "blue", "Concentration of C", "Conversion", "Concentration");
    graphCalculator('plot4', DataD, "magenta", "Concentration of D", "Conversion", "Concentration");
    graphCalculator('plot5', DataRa, "green", "Rate of A", "Conversion", "Rate");
    graphCalculator('plot6', DataRb, "red", "Rate of B", "Conversion", "Rate");
    graphCalculator('plot7', DataRc, "blue", "Rate of C", "Conversion", "Rate");
    graphCalculator('plot8', DataRd, "magenta", "Rate of D", "Conversion", "Rate");
    graphCalculator('plot9', DataFx, "black", "Levenspiel plot", "Conversion", "F0/-rA");
    //Volume
    let Volume = 0;
    if (reactor === 'PFR' || reactor === 'PBR') {
        let dx = X / 20;
        for (let x = 0; x <= X; x += dx) {
            let r = k * (ConcA(x) ** p) * (ConcB(x) ** q);
            if (r > 0) Volume += dx / r;
        }
        Volume = Volume * flowRate * cia;
    }
    else if (reactor === 'CSTR') {
        Volume = flowRate * cia * X / (k * (ConcA(X) ** p) * (ConcB(X) ** q));
    }
    document.querySelector('#message').innerHTML += "<br>-> The volume required for " + reactor + " is " + Volume + " litres";
    //Pressure
    if (phase === "gas" && pressure === 'variableP') {
        let P = Px(X);
        document.querySelector('#message').innerHTML += "<br>-> The final pressure is " + P + " atm";        
    }
}

function graphCalculator(id, DataPoints, color, title, titleX, titleY) {
    // debugger;
    Plotly.newPlot(id, [{
        x: DataPoints.map(point => point.x),
        y: DataPoints.map(point => point.y),
        type: 'scatter',
        mode: 'lines',
        line: {
            color: color
        }
    }], {
        title: title,
        xaxis: {
            title: titleX
        },
        yaxis: {
            title: titleY
        }
    });
}