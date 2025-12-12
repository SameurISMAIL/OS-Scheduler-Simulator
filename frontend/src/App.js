import React, { useState, useEffect } from 'react';
import GanttChart from './components/GanttChart';
import Metrics from './components/Metrics';
import LogPanel from './components/LogPanel';
import ProcessTable from './components/ProcessTable';
import './App.css';

function App() {
    const [algorithms, setAlgorithms] = useState([]);
    const [selectedAlgorithm, setSelectedAlgorithm] = useState('');
    
    const [processes, setProcesses] = useState([]); 

    const [quantum, setQuantum] = useState(2);
    
    const [multiLevelType, setMultiLevelType] = useState('STATIC');

    const [simulationResult, setSimulationResult] = useState(null);
    const [isLoading, setIsLoading] = useState(true);
    const [error, setError] = useState(null);
    const [isInitialLoad, setIsInitialLoad] = useState(true);

    const parseProcessData = (text) => {
        const lines = text.split('\n');
        const parsed = [];
        let idCounter = 0;
        
        lines.forEach(line => {
            const trimmed = line.trim();
            if (trimmed && !trimmed.startsWith('#') && !trimmed.startsWith('PID')) {

                const parts = trimmed.replace(/,/g, ' ').split(/\s+/);
                if (parts.length >= 4) {
                    parsed.push({
                        id: idCounter++,
                        name: parts[0],
                        arrival: parseInt(parts[1], 10),
                        burst: parseInt(parts[2], 10),
                        priority: parseInt(parts[3], 10)
                    });
                }
            }
        });
        return parsed;
    };

    const generateProcessString = (procs) => {
        let str = "# PID, Arrival, Burst, Priority\n";
        procs.forEach(p => {
            str += `${p.name}, ${p.arrival}, ${p.burst}, ${p.priority}\n`;
        });
        return str;
    };

    useEffect(() => {
        Promise.all([
            fetch('http://localhost:3001/api/algorithms'),
            fetch('/params.txt')
        ])
        .then(([algosRes, paramsRes]) => {
            if (!algosRes.ok) throw new Error("Failed to fetch algorithms from backend.");
            if (!paramsRes.ok) throw new Error("Failed to fetch params.txt.");
            return Promise.all([algosRes.json(), paramsRes.text()]);
        })
        .then(([algosData, paramsText]) => {

            let cleanList = algosData.filter(name => 
                name !== 'multilevel_static' && 
                name !== 'multilevel_dynamic'
            );
            
            cleanList.push('Multilevel');
            const sortedData = cleanList.sort();

            setAlgorithms(sortedData);
            

            const initialProcesses = parseProcessData(paramsText);
            setProcesses(initialProcesses);

            if (sortedData.includes('FIFO')) {
                setSelectedAlgorithm('FIFO');
            } else if (sortedData.length > 0) {
                setSelectedAlgorithm(sortedData[0]);
            }
        })
        .catch((err) => {
            console.error("Initialization Error:", err);
            setError(err.message || "Could not load initial data.");
            setIsLoading(false);
        });
    }, []);

    const runSimulation = () => {
        setIsLoading(true); 
        setSimulationResult(null);
        setError(null);


        const processConfigString = generateProcessString(processes);


        let backendAlgo = selectedAlgorithm;
        if (selectedAlgorithm === 'Multilevel') {
            backendAlgo = (multiLevelType === 'STATIC') ? 'multilevel_static' : 'multilevel_dynamic';
        }

        let payload = {
            algorithm: backendAlgo,
            processes: processConfigString, 
        };

        if (selectedAlgorithm === 'RR' || selectedAlgorithm === 'Multilevel') {
            payload.quantum = quantum;
        }

        fetch('http://localhost:3001/api/run', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(payload)
        })
        .then(res => res.ok ? res.json() : res.json().then(err => Promise.reject(err)))
        .then(data => {
            setSimulationResult(data);
            setIsLoading(false);
        })
        .catch(err => {
            setError(err.error || 'Simulation failed. Check console.');
            setIsLoading(false);
        });
    };
    

    useEffect(() => {
        if (isInitialLoad && algorithms.length > 0 && processes.length > 0) {
            const defaultAlgorithm = "FIFO";
            if (algorithms.includes(defaultAlgorithm)) {

                const configStr = generateProcessString(processes);

                 fetch('http://localhost:3001/api/run', {
                    method: 'POST',
                    headers: { 'Content-Type': 'application/json' },
                    body: JSON.stringify({ algorithm: defaultAlgorithm, processes: configStr })
                })
                .then(res => res.json())
                .then(data => {
                    setSimulationResult(data);
                    setIsLoading(false);
                })
                .catch(e => setIsLoading(false));
            }
            setIsInitialLoad(false);
        }

    }, [algorithms, processes, isInitialLoad]);

    return (
        <div className="container">
            <header>
                <div className="logo">
                    <img src="/cpu.ico" width="50" height="50" alt="CPU Icon" />
                    <h1>OS Process Scheduler Simulator</h1>
                </div>
            </header>
            <main className="main-grid">
                <div className="controls-panel">
                    <div className="card">
                        <div className="form-group">
                            <label htmlFor="algo-select">Scheduling Algorithm</label>
                            <select id="algo-select" value={selectedAlgorithm} onChange={e => setSelectedAlgorithm(e.target.value)}>
                                {algorithms.map(algo => <option key={algo} value={algo}>{algo}</option>)}
                            </select>
                        </div>
                        
                        {selectedAlgorithm === 'RR' && (
                            <div className="form-group">
                                <label htmlFor="quantum-input">Quantum Time Slice (ms)</label>
                                <input
                                    type="number"
                                    id="quantum-input"
                                    value={quantum}
                                    onChange={e => setQuantum(Number(e.target.value))}
                                    min="1"
                                />
                            </div>
                        )}

                        {selectedAlgorithm === 'Multilevel' && (
                            <div className="multilevel-options">
                                <div className="form-group checkbox-group">
                                    <label style={{cursor: 'pointer'}}>
                                        <input 
                                            type="checkbox" 
                                            checked={multiLevelType === 'STATIC'}
                                            onChange={() => setMultiLevelType('STATIC')}
                                        />
                                        <span style={{marginLeft: '8px'}}>Static Multi-Level</span>
                                    </label>
                                    <label style={{cursor: 'pointer'}}>
                                        <input 
                                            type="checkbox" 
                                            checked={multiLevelType === 'DYNAMIC'}
                                            onChange={() => setMultiLevelType('DYNAMIC')}
                                        />
                                        <span style={{marginLeft: '8px'}}>Dynamic Multi-Level</span>
                                    </label>
                                </div>
                                <div className="form-group">
                                    <label htmlFor="quantum-input-ml">Quantum (ms)</label>
                                    <input
                                        type="number"
                                        id="quantum-input-ml"
                                        value={quantum}
                                        onChange={e => setQuantum(Number(e.target.value))}
                                        min="1"
                                    />
                                </div>
                            </div>
                        )}
                        
                        <button 
                            className="btn-run" 
                            onClick={runSimulation} 
                            disabled={isLoading || !selectedAlgorithm}>
                            {isLoading ? 'Simulating...' : 'Run Simulation'}
                        </button>
                    </div>
                    
                    {}
                    <div className="card">
                        <label>Process Configuration</label>
                        <ProcessTable 
                            processes={processes} 
                            setProcesses={setProcesses} 
                        />
                    </div>

                </div>
                <div className="results-panel">
                    {error && <div className="card error-panel">Error: {error}</div>}
                    {!simulationResult && isLoading && (
                        <div className="card placeholder">
                           <div>
                                <h2>{isInitialLoad ? "Loading Initial Data..." : "Simulating..."}</h2>
                           </div>
                        </div>
                    )}
                    {!simulationResult && !isLoading && !error && (
                        <div className="card placeholder">
                            <div>
                                <h2>Welcome!</h2>
                                <p>All data loaded. Ready to run a new simulation.</p>
                            </div>
                        </div>
                    )}
                    {simulationResult && (
                        <>
                            <GanttChart data={simulationResult} />
                            <Metrics data={simulationResult} />
                            <LogPanel logs={simulationResult.logs} />
                        </>
                    )}
                </div>
            </main>
        </div>
    );
}

export default App;