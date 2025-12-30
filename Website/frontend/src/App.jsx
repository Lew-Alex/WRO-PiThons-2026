import React, { useEffect, useRef, useState } from "react";
import fieldImage from "./assets/field.png";

function App() {
  const canvasRef = useRef(null);
  const [point, setPoint] = useState({ x: 100, y: 100 });
  const [dragging, setDragging] = useState(false);

  useEffect(() => {
    const canvas = canvasRef.current;
    const ctx = canvas.getContext("2d");
    const img = new Image();
    img.src = fieldImage;
    img.onload = () => {
      canvas.width = img.width;
      canvas.height = img.height;
      draw();
    };

    function draw() {
      ctx.clearRect(0, 0, canvas.width, canvas.height);
      ctx.fillStyle = "red";
      ctx.beginPath();
      ctx.arc(point.x, point.y, 10, 0, Math.PI * 2);
      ctx.fill();
    }

    function getMousePos(evt) {
      const rect = canvas.getBoundingClientRect();
      return {
        x: evt.clientX - rect.left,
        y: evt.clientY - rect.top,
      };
    }

    function onMouseDown(evt) {
      const pos = getMousePos(evt);
      const dx = pos.x - point.x;
      const dy = pos.y - point.y;
      if (Math.sqrt(dx * dx + dy * dy) < 10) setDragging(true);
    }

    function onMouseMove(evt) {
      if (!dragging) return;
      const pos = getMousePos(evt);
      setPoint({ x: pos.x, y: pos.y });
      draw();
    }

    function onMouseUp() {
      setDragging(false);
    }

    canvas.addEventListener("mousedown", onMouseDown);
    canvas.addEventListener("mousemove", onMouseMove);
    canvas.addEventListener("mouseup", onMouseUp);
    canvas.addEventListener("mouseleave", onMouseUp);

    return () => {
      canvas.removeEventListener("mousedown", onMouseDown);
      canvas.removeEventListener("mousemove", onMouseMove);
      canvas.removeEventListener("mouseup", onMouseUp);
      canvas.removeEventListener("mouseleave", onMouseUp);
    };
  }, [point, dragging]);

  return (
    <div
      style={{
        position: "relative",
        width: "100vw",
        height: "100vh",
        overflow: "hidden",
        padding: "10px",
        boxSizing: "border-box",
      }}
    >
      <img
        src={fieldImage}
        alt="Field"
        style={{
          display: "block",
          width: "60vw",
          maxHeight: "calc(100vh - 20px)",
          objectFit: "contain",
        }}
      />
      <canvas
        ref={canvasRef}
        style={{
          position: "absolute",
          top: "10px",
          left: "10px",
          pointerEvents: "auto",
        }}
      />
    </div>
  );
}

export default App;