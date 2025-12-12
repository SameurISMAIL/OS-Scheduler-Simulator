import React from 'react';
import './GanttChart.css';

const getColor = (pid) => {
    const colors = ['#e74c3c', '#3498db', '#2ecc71', '#f1c40f', '#9b59b6', '#1abc9c'];
    const index = parseInt(pid.replace('P', ''), 10);
    return colors[(index - 1) % colors.length];
};

const GanttChart = ({ data }) => {
    if (!data || !data.ganttChart || data.ganttChart.length === 0) return null;
    const totalTime = data.ganttChart.reduce((max, block) => Math.max(max, block.end), 0);
    const scale = totalTime > 0 ? 100 / totalTime : 0;

    return (
        <div className="card gantt-card">
            <h3 className="card-title">Gantt Chart</h3>
            <div className="gantt-chart-area">
                <div className="gantt-chart-bar">
                    {data.ganttChart.map((block, index) => (
                        <div
                            key={index}
                            className="gantt-block"
                            style={{
                                left: `${block.start * scale}%`,
                                width: `${(block.end - block.start) * scale}%`,
                                backgroundColor: getColor(block.pid),
                            }}
                            title={`Process ${block.pid} (${block.start} to ${block.end})`}
                        >
                            {block.pid}
                        </div>
                    ))}
                </div>
                <div className="timeline">
                    {[...Array(totalTime + 1).keys()].filter(t => t % 1 === 0 || t === 0 || t === totalTime).map(time => (
                         <div key={time} className="time-marker" style={{ left: `${time * scale}%` }}>
                           <span>{time}</span>
                         </div>
                      ))}
                </div>
            </div>
        </div>
    );
};
export default GanttChart;