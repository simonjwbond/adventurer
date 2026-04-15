#!/usr/bin/env python3
"""Generate proper 48x64 clothing layer for blue jumpsuit."""

WIDTH = 48
HEIGHT = 64

def generate_clothing_row(y):
    """Generate a single row of the clothing layer."""
    row = [0] * WIDTH
    
    # Jumpsuit starts at row 21 (shoulders) and goes to bottom
    if y >= 21:
        # Calculate body width at this height
        if y <= 42:
            # Torso section - wider
            outline_x = 10 + (y - 23) // 4
        else:
            # Legs section - narrower
            outline_x = 10 + (y - 43) // 6
        
        # Draw jumpsuit color (7 = blue) between outlines
        for x in range(outline_x + 1, WIDTH - 1 - outline_x):
            row[x] = 7
        
        # Add collar detail at top of jumpsuit (row 21-23)
        if y >= 21 and y <= 23:
            collar_center = WIDTH // 2
            for x in range(collar_center - 2, collar_center + 3):
                row[x] = 8  # zipper color
        
        # Add vertical zipper line (rows 24-40)
        if y > 23 and y < 41:
            row[WIDTH // 2 - 1] = 8
            row[WIDTH // 2] = 8
    
    return row

# Generate all rows
print("=== CLOTHING_TUNIC ===")
for y in range(HEIGHT):
    row = generate_clothing_row(y)
    print(",".join(map(str, row)) + ",")
