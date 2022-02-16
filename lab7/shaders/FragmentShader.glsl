#version 410

// Input
layout(location = 0) in vec2 texture_coord;

// Uniform properties
uniform sampler2D textureImage;
uniform ivec2 screenSize;
uniform int flipVertical;
uniform int outputMode = 2; // 0: original, 1: grayscale, 2: blur

// Output
layout(location = 0) out vec4 out_color;

// Local variables
vec2 textureCoord = vec2(texture_coord.x, (flipVertical != 0) ? 1 - texture_coord.y : texture_coord.y); // Flip texture


vec4 grayscale()
{
    vec4 color = texture(textureImage, textureCoord);
    float gray = 0.21 * color.r + 0.71 * color.g + 0.07 * color.b; 
    return vec4(gray, gray, gray,  0);
}


vec4 blur(int blurRadius)
{
    vec2 texelSize = 1.0f / screenSize;
    vec4 sum = vec4(0);
    for(int i = -blurRadius; i <= blurRadius; i++)
    {
        for(int j = -blurRadius; j <= blurRadius; j++)
        {
            sum += texture(textureImage, textureCoord + vec2(i, j) * texelSize);
        }
    }
        
    float samples = pow((2 * blurRadius + 1), 2);
    return sum / samples;
}

// FILTRU MEDIE
vec4 medie(){
	vec2 texelSize = 1.0f / screenSize;
	vec4 color = texture(textureImage, textureCoord);
	vec4 sum = vec4(0);
	if (textureCoord.x==0||textureCoord.y==0||textureCoord.y==screenSize.y-1||textureCoord.x==screenSize.x-1) {
		return vec4(color.r,color.g,color.b,0);
	} else {
		for(int i = -1; i <= 1; i++)
		{
			for(int j = -1; j <= 1; j++)
			{
				sum += texture(textureImage, textureCoord + vec2(i, j) * texelSize);
			}
		}
		sum -= color;
		return sum/8;
	}
}

// FILTRU MEDIAN
float avg_gray(vec4 pixel)
{
	return (pixel.r + pixel.g + pixel.b) / 3;
}

vec4 median()
{
	const int radius = 2;
	vec4 to_sort[25];
	vec2 texelSize = 1.0f / screenSize;

	for(int i = -radius; i <= radius; i++) {
		for(int j = -radius; j <= radius; j++) {
			to_sort[(2 * radius + 1) * (i + 2) + (j + 2)] = texture(textureImage, textureCoord + vec2(i, j) * texelSize);
		}
	}
	
	for (int i = 0; i < (2 * radius + 1) * (2 * radius + 1); ++i) {
		for (int j = 0; j <= (2 * radius + 1) * (2 * radius + 1); ++j) {
			if (avg_gray(to_sort[i]) > avg_gray(to_sort[j])) {
				vec4 aux = to_sort[i];
				to_sort[i] = to_sort[j];
				to_sort[j] = aux;
			}
		}
	}

	return to_sort[(2 * radius + 1) * (2 * radius + 1) / 2];
}

// FILTRU GAUSSIAN
vec4 gaussian()
{
	vec2 texelSize = 1.0f / screenSize;
	vec4 sum = vec4(0);
	float to_divide = 273;
	float kernel[25] = float[](1, 4, 7, 4, 1, 4, 16, 26, 16, 4, 7, 26, 41, 26, 7, 4, 16, 26, 16, 4, 1, 4, 7, 4, 1);

	for(int i = -2; i <= 2; i++) {
		for(int j = -2; j <= 2; j++) {
			sum += texture(textureImage, textureCoord + vec2(i, j) * texelSize) * kernel[5 * (i + 2) + (j + 2)];
		}
	}
	return sum / to_divide;
}

// SOBEL
vec4 sobel()
{
	vec2 texelSize = 1.0f / screenSize;
	float gx[9] = float[](1, 0, -1, 2, 0, -2, 1, 0, -1);
	float gy[9] = float[](1, 2, 1, 0, 0, 0, -1, -2, -1);
	vec4 dx = vec4(0);
	vec4 dy = vec4(0);

	for(int i = -1; i <= 1; i++)
	{
		for(int j = -1; j <= 1; j++)
		{
			dx += texture(textureImage, textureCoord + vec2(i, j) * texelSize) * gx[3 * (i + 1) + (j + 1)];
		}
	}

		for(int i = -1; i <= 1; i++)
	{
		for(int j = -1; j <= 1; j++)
		{
			dy += texture(textureImage, textureCoord + vec2(i, j) * texelSize) * gy[3 * (i + 1) + (j + 1)];
		}
	}

	return sqrt(dx*dx + dy*dy);
}

// PREWITT
vec4 prewitt()
{
	vec2 texelSize = 1.0f / screenSize;
	float gx[9] = float[](1, 0, -1, 1, 0, -1, 1, 0, -1);
	float gy[9] = float[](1, 1, 1, 0, 0, 0, -1, -1, -1);
	vec4 dx = vec4(0);
	vec4 dy = vec4(0);

	for(int i = -1; i <= 1; i++)
	{
		for(int j = -1; j <= 1; j++)
		{
			dx += texture(textureImage, textureCoord + vec2(i, j) * texelSize) * gx[3 * (i + 1) + (j + 1)];
		}
	}

		for(int i = -1; i <= 1; i++)
	{
		for(int j = -1; j <= 1; j++)
		{
			dy += texture(textureImage, textureCoord + vec2(i, j) * texelSize) * gy[3 * (i + 1) + (j + 1)];
		}
	}

	return sqrt(dx*dx + dy*dy);
}

// ROBERTS
vec4 roberts()
{
	vec2 texelSize = 1.0f / screenSize;
	float gx[9] = float[](0, 0, 0, 0, 1, 0, 0, 0, -1);
	float gy[9] = float[](0, 0, 0, 0, 0, 1, 0, -1, 0);
	vec4 dx = vec4(0);
	vec4 dy = vec4(0);

	for(int i = -1; i <= 1; i++)
	{
		for(int j = -1; j <= 1; j++)
		{
			dx += texture(textureImage, textureCoord + vec2(i, j) * texelSize) * gx[3 * (i + 1) + (j + 1)];
		}
	}

		for(int i = -1; i <= 1; i++)
	{
		for(int j = -1; j <= 1; j++)
		{
			dy += texture(textureImage, textureCoord + vec2(i, j) * texelSize) * gy[3 * (i + 1) + (j + 1)];
		}
	}

	return sqrt(dx*dx + dy*dy);
}



void main()
{
    switch (outputMode)
    {
        case 1:
        {
            out_color = grayscale();
            break;
        }

        case 2:
        {
            out_color = blur(3);
            break;
        }

        case 3:
        {
            out_color = medie();
			break;
        }

        case 4:
        {
            out_color = median();
			break;
        }
        case 5:
		{
			out_color = gaussian();
			break;
		}
		case 6:
		{
			out_color = sobel();
			break;
		}
		case 7:
		{
			out_color = prewitt();
			break;
		}
		case 8:
		{
			out_color = roberts();
			break;
		}

        default:
            out_color = texture(textureImage, textureCoord);
            break;
    }
}
