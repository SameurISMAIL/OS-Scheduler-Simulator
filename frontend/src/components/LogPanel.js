import React, { useEffect, useRef } from 'react';
import './LogPanel.css';

const LogPanel = ({ logs }) => {
    const logEndRef = useRef(null);

    useEffect(() => {
        if (logEndRef.current) {
            logEndRef.current.scrollIntoView({ behavior: "smooth" });
        }
    }, [logs]);

    if (!logs || logs.length === 0) return null;

    return (
        <div className="card log-card">
            <h3 className="card-title">Simulation Logs</h3>
            <div className="log-console-window">
                {logs.map((log, index) => (
                    <div key={index} className="log-line">
                        <span className="log-arrow">&gt;</span> {log}
                    </div>
                ))}
                <div ref={logEndRef} />
            </div>
        </div>
    );
};

export default LogPanel;