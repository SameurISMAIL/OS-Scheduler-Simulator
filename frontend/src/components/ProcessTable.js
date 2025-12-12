import React from 'react';
import './ProcessTable.css';

const ProcessTable = ({ processes, setProcesses }) => {

    const handleChange = (index, field, value) => {
        const updatedProcesses = [...processes];
        updatedProcesses[index][field] = value;
        setProcesses(updatedProcesses);
    };

    const addProcess = () => {
        const newId = processes.length > 0 ? processes.length + 1 : 1;
        setProcesses([
            ...processes,
            { id: Date.now(), name: `P${newId}`, arrival: 0, burst: 1, priority: 1 }
        ]);
    };

    const removeProcess = (index) => {
        const updatedProcesses = processes.filter((_, i) => i !== index);
        setProcesses(updatedProcesses);
    };

    return (
        <div className="process-table-container">
            <table className="process-table">
                <thead>
                    <tr>
                        <th>Process Name</th>
                        <th>Arrival Time</th>
                        <th>Burst Time</th>
                        <th>Priority</th>
                        <th>Action</th>
                    </tr>
                </thead>
                <tbody>
                    {processes.map((proc, index) => (
                        <tr key={proc.id || index}>
                            <td>
                                <input
                                    type="text"
                                    value={proc.name}
                                    onChange={(e) => handleChange(index, 'name', e.target.value)}
                                    className="table-input"
                                />
                            </td>
                            <td>
                                <input
                                    type="number"
                                    min="0"
                                    value={proc.arrival}
                                    onChange={(e) => handleChange(index, 'arrival', parseInt(e.target.value) || 0)}
                                    className="table-input"
                                />
                            </td>
                            <td>
                                <input
                                    type="number"
                                    min="1"
                                    value={proc.burst}
                                    onChange={(e) => handleChange(index, 'burst', parseInt(e.target.value) || 1)}
                                    className="table-input"
                                />
                            </td>
                            <td>
                                <input
                                    type="number"
                                    min="0"
                                    value={proc.priority}
                                    onChange={(e) => handleChange(index, 'priority', parseInt(e.target.value) || 0)}
                                    className="table-input"
                                />
                            </td>
                            <td>
                                <button 
                                    className="btn-delete"
                                    onClick={() => removeProcess(index)}
                                    title="Remove Process"
                                >
                                    ✕
                                </button>
                            </td>
                        </tr>
                    ))}
                </tbody>
            </table>
            <button className="btn-add" onClick={addProcess}>
                + Add Process
            </button>
        </div>
    );
};

export default ProcessTable;