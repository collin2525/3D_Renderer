import pygame
import os
import ctypes
import math
from ctypes import c_int, c_double, c_char_p, POINTER, byref

# Load the C library
renderer_path = os.path.abspath("./mylib.dll")
renderer = ctypes.CDLL(renderer_path)

# Define function signatures
renderer.initialize_renderer.argtypes = [c_int, c_int, c_double]
renderer.initialize_renderer.restype = None

renderer.get_projected_vertices.argtypes = [POINTER(c_int), c_char_p]
renderer.get_projected_vertices.restype = POINTER(c_double)

# Initialize Pygame
pygame.init()
screen = pygame.display.set_mode((800, 600))
pygame.display.set_caption("3D Cube Renderer")
clock = pygame.time.Clock()

# Initialize the renderer with a cube
renderer.initialize_renderer(800, 600, 90.0)

running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    # Clear the screen
    screen.fill((0, 0, 0))

    # Get projected vertices for the cube
    count = c_int()
    projected_ptr = renderer.get_projected_vertices(byref(count), b"cube")
    
    if projected_ptr and count.value > 0:
        # Convert to Python list
        projected = [projected_ptr[i] for i in range(count.value)]
        
        # Draw each edge
        for i in range(0, len(projected), 4):
            x1, y1, x2, y2 = projected[i:i+4]
            pygame.draw.line(screen, (255, 255, 255), (x1, y1), (x2, y2), 2)

    # Update the display
    pygame.display.flip()
    clock.tick(60)

pygame.quit()

