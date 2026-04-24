import matplotlib.pyplot as plt
import re

# Load image
img = plt.imread("map_downscaled.png")

# Load log file
with open("log.txt", "r") as f:
    lines = f.readlines()

real_positions = []
estimate_positions = []
particle_sets = []

# Parse log
for line in lines:
    if "REAL" in line:
        match = re.search(r"X:\s*([-\d\.]+)\s*Y:\s*([-\d\.]+)", line)
        if match:
            real_positions.append((float(match.group(1)), float(match.group(2))))

    elif "ESTIMATE" in line:
        match = re.search(r"X:\s*([-\d\.]+)\s*Y:\s*([-\d\.]+)", line)
        if match:
            estimate_positions.append((float(match.group(1)), float(match.group(2))))

    elif "PARTICLES" in line:
        particles = re.findall(r"X:\s*([-\d\.]+)\s*Y:\s*([-\d\.]+)", line)
        particle_sets.append([(float(x), float(y)) for x, y in particles])

# Animate
plt.ion()
fig, ax = plt.subplots()

for i in range(len(particle_sets)):
    ax.clear()

    # 🗺️ Background image (IMPORTANT: before scatter)
    ax.imshow(img, extent=[0, 2362, 0, 1143], origin='lower')

    # Particles
    px = [p[0] for p in particle_sets[i]]
    py = [p[1] for p in particle_sets[i]]
    ax.scatter(px, py, s=10)

    # REAL
    if i < len(real_positions):
        rx, ry = real_positions[i]
        ax.scatter([rx], [ry], s=60)

    # ESTIMATE
    if i < len(estimate_positions):
        ex, ey = estimate_positions[i]
        ax.scatter([ex], [ey], s=60)

    # Fixed field
    ax.set_xlim(0, 2362)
    ax.set_ylim(0, 1143)

    # Clean look
    ax.axis('off')

    plt.pause(0.05)

plt.ioff()
plt.show()