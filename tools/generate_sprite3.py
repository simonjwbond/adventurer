#!/usr/bin/env python3
"""Generate proper 48x64 pixel art for hero - blonde hair, blue jumpsuit."""

WIDTH = 48
HEIGHT = 64

def generate_row(y):
    """Generate a single row of the down-facing sprite."""
    row = [0] * WIDTH
    
    # Hair (rows 0-7) - messy blonde on top
    if y <= 2:
        w = 18 + y * 3
        start = (WIDTH - w) // 2
        for x in range(start, min(start + w, WIDTH)):
            row[x] = 2
    elif y <= 5:
        w = 24 + y * 2
        start = (WIDTH - w) // 2
        for x in range(start, min(start + w, WIDTH)):
            row[x] = 2
    elif y <= 7:
        hair_w = 6
        left_start = max(0, 10 - y)
        right_start = min(WIDTH - hair_w, WIDTH - 10 + y)
        for x in range(left_start, min(left_start + hair_w, WIDTH)):
            row[x] = 2
        for x in range(right_start, min(right_start + hair_w, WIDTH)):
            row[x] = 2
    
    # Face (rows 8-15)
    elif y <= 15:
        outline_x = 6 + (y - 8) // 2
        if outline_x < WIDTH:
            row[outline_x] = 3
        if WIDTH - 1 - outline_x >= 0 and WIDTH - 1 - outline_x < WIDTH:
            row[WIDTH - 1 - outline_x] = 3
        
        if y >= 10 and y <= 12:
            eye_pos = WIDTH // 2
            # Left eye
            for i, offset in enumerate([-8, -7, -6, -5, -4]):
                idx = eye_pos + offset
                if 0 <= idx < WIDTH:
                    row[idx] = [3, 4, 5, 4, 3][i]
            # Right eye
            for i, offset in enumerate([4, 5, 6, 7, 8]):
                idx = eye_pos + offset
                if 0 <= idx < WIDTH:
                    row[idx] = [3, 4, 5, 4, 3][i]
        else:
            face_w = 20
            start = (WIDTH - face_w) // 2
            for x in range(start, min(start + face_w, WIDTH)):
                if row[x] == 0:
                    row[x] = 1
    
    # Neck and shoulders (rows 16-22)
    elif y <= 22:
        outline_x = 8 + (y - 16) // 2
        if outline_x < WIDTH:
            row[outline_x] = 3
        if WIDTH - 1 - outline_x >= 0 and WIDTH - 1 - outline_x < WIDTH:
            row[WIDTH - 1 - outline_x] = 3
        for x in range(outline_x + 1, WIDTH - 1 - outline_x):
            if row[x] == 0:
                row[x] = 1
    
    # Torso (rows 23-42)
    elif y <= 42:
        outline_x = 10 + (y - 23) // 4
        if outline_x < WIDTH:
            row[outline_x] = 3
        if WIDTH - 1 - outline_x >= 0 and WIDTH - 1 - outline_x < WIDTH:
            row[WIDTH - 1 - outline_x] = 3
        for x in range(outline_x + 1, WIDTH - 1 - outline_x):
            if row[x] == 0:
                row[x] = 1
    
    # Legs (rows 43-58)
    elif y <= 58:
        leg_w = max(6, 16 - (y - 43) // 2)
        start = (WIDTH - leg_w * 2) // 2
        if start < WIDTH:
            row[start] = 3
        if start + leg_w * 2 - 1 >= 0 and start + leg_w * 2 - 1 < WIDTH:
            row[start + leg_w * 2 - 1] = 3
        for x in range(start + 1, min(start + leg_w * 2 - 1, WIDTH)):
            if row[x] == 0:
                row[x] = 1
    
    # Feet (rows 59-63)
    else:
        foot_w = max(4, 8 - (y - 59))
        start = (WIDTH - foot_w * 2) // 2
        for x in range(start, min(start + foot_w * 2, WIDTH)):
            if row[x] == 0:
                row[x] = 1
    
    return row

# Generate all rows with proper C array formatting (no trailing comma on last element)
print("=== BODY_DATA ===")
for y in range(HEIGHT):
    row = generate_row(y)
    # Format as comma-separated values without trailing comma
    print(",".join(map(str, row)) + ",")
