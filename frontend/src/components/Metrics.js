import React from 'react';
import './Metrics.css';

const MetricCard = ({ title, value, unit }) => (
    <div className="metric-card">
        <div className="metric-title">{title}</div>
        <div className="metric-value">
            {value} <span className="metric-unit">{unit}</span>
        </div>
    </div>
);

const Metrics = ({ data }) => {
    if (!data) return null;
    const { metrics } = data;
    return (
        <div className="card metrics-section">
            <h3 className="card-title">Performance Metrics</h3>
            <div className="metrics-row">
                <MetricCard 
                    title="Avg. Waiting Time" 
                    value={metrics.avgWaitingTime.toFixed(2)} 
                    unit="ms" 
                />
                <MetricCard 
                    title="Avg. Turnaround Time" 
                    value={metrics.avgTurnaroundTime.toFixed(2)} 
                    unit="ms" 
                />
                <MetricCard 
                    title="Throughput" 
                    value={metrics.throughput.toFixed(3)} 
                    unit="p/ms" 
                />
            </div>
        </div>
    );
};
export default Metrics;