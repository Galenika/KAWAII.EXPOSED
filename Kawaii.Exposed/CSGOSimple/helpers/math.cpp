#include "Math.hpp"

namespace Math
{
	QAngle CalcAngle(Vector src, Vector dst)
	{
		QAngle angles;
		Vector delta = src - dst;

		Math::VectorAngles(delta, angles);

		delta.Normalized();

		return angles;
	}

	void CorrectMovement(QAngle vOldAngles, CUserCmd* pCmd, float fOldForward, float fOldSidemove)
	{
		// side/forward move correction
		float deltaView;
		float f1;
		float f2;

		if (vOldAngles.yaw < 0.f)
			f1 = 360.0f + vOldAngles.yaw;
		else
			f1 = vOldAngles.yaw;

		if (pCmd->viewangles.yaw < 0.0f)
			f2 = 360.0f + pCmd->viewangles.yaw;
		else
			f2 = pCmd->viewangles.yaw;

		if (f2 < f1)
			deltaView = abs(f2 - f1);
		else
			deltaView = 360.0f - abs(f1 - f2);

		deltaView = 360.0f - deltaView;

		pCmd->forwardmove = cos(DEG2RAD(deltaView)) * fOldForward + cos(DEG2RAD(deltaView + 90.f)) * fOldSidemove;
		pCmd->sidemove = sin(DEG2RAD(deltaView)) * fOldForward + sin(DEG2RAD(deltaView + 90.f)) * fOldSidemove;
	}
	//--------------------------------------------------------------------------------
    void NormalizeAngles(QAngle& angles)
    {
        for(auto i = 0; i < 3; i++) 
		{
            while(angles[i] < -180.0f) angles[i] += 360.0f;
            while(angles[i] >  180.0f) angles[i] -= 360.0f;
        }
    }


	void NormalizeAnglesVector(Vector& angles)
	{
		for (auto i = 0; i < 3; i++)
		{
			while (angles[i] < -180.0f) angles[i] += 360.0f;
			while (angles[i] >  180.0f) angles[i] -= 360.0f;
		}
	}

    //--------------------------------------------------------------------------------
    void ClampAngles(QAngle& angles)
    {
        if(angles.pitch > 89.0f) angles.pitch = 89.0f;
        else if(angles.pitch < -89.0f) angles.pitch = -89.0f;

        if(angles.yaw > 180.0f) angles.yaw = 180.0f;
        else if(angles.yaw < -180.0f) angles.yaw = -180.0f;

        angles.roll = 0;
    }
    //--------------------------------------------------------------------------------
    void VectorTransform(const Vector& in1, const matrix3x4_t& in2, Vector& out)
    {
        out[0] = in1.Dot(in2[0]) + in2[0][3];
        out[1] = in1.Dot(in2[1]) + in2[1][3];
        out[2] = in1.Dot(in2[2]) + in2[2][3];
    }
    //--------------------------------------------------------------------------------
    void AngleVectors2(const QAngle &angles, Vector& forward)
    {
        float	sp, sy, cp, cy;

        DirectX::XMScalarSinCos(&sp, &cp, DEG2RAD(angles[0]));
        DirectX::XMScalarSinCos(&sy, &cy, DEG2RAD(angles[1]));

        forward.x = cp*cy;
        forward.y = cp*sy;
        forward.z = -sp;
    }
    //--------------------------------------------------------------------------------

	void angle_vectors2(const Vector &angles, Vector *forward, Vector *right, Vector *up)
	{
		float sr, sp, sy, cr, cp, cy;

		sp = static_cast<float>(sin(double(angles.x) * PIRAD));
		cp = static_cast<float>(cos(double(angles.x) * PIRAD));
		sy = static_cast<float>(sin(double(angles.y) * PIRAD));
		cy = static_cast<float>(cos(double(angles.y) * PIRAD));
		sr = static_cast<float>(sin(double(angles.z) * PIRAD));
		cr = static_cast<float>(cos(double(angles.z) * PIRAD));

		if (forward)
		{
			forward->x = cp*cy;
			forward->y = cp*sy;
			forward->z = -sp;
		}

		if (right)
		{
			right->x = (-1 * sr*sp*cy + -1 * cr*-sy);
			right->y = (-1 * sr*sp*sy + -1 * cr*cy);
			right->z = -1 * sr*cp;
		}

		if (up)
		{
			up->x = (cr*sp*cy + -sr*-sy);
			up->y = (cr*sp*sy + -sr*cy);
			up->z = cr*cp;
		}
	}

    void AngleVectors(const QAngle &angles, Vector& forward, Vector& right, Vector& up)
    {
        float sr, sp, sy, cr, cp, cy;

        DirectX::XMScalarSinCos(&sp, &cp, DEG2RAD(angles[0]));
        DirectX::XMScalarSinCos(&sy, &cy, DEG2RAD(angles[1]));
        DirectX::XMScalarSinCos(&sr, &cr, DEG2RAD(angles[2]));

        forward.x = (cp * cy);
        forward.y = (cp * sy);
        forward.z = (-sp);
        right.x = (-1 * sr * sp * cy + -1 * cr * -sy);
        right.y = (-1 * sr * sp * sy + -1 * cr *  cy);
        right.z = (-1 * sr * cp);
        up.x = (cr * sp * cy + -sr*-sy);
        up.y = (cr * sp * sy + -sr*cy);
        up.z = (cr * cp);
    }
    //--------------------------------------------------------------------------------
    void VectorAngles(const Vector& forward, QAngle& angles)
    {
        float	tmp, yaw, pitch;

        if(forward[1] == 0 && forward[0] == 0) {
            yaw = 0;
            if(forward[2] > 0)
                pitch = 270;
            else
                pitch = 90;
        } else {
            yaw = (atan2(forward[1], forward[0]) * 180 / DirectX::XM_PI);
            if(yaw < 0)
                yaw += 360;

            tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
            pitch = (atan2(-forward[2], tmp) * 180 / DirectX::XM_PI);
            if(pitch < 0)
                pitch += 360;
        }

        angles[0] = pitch;
        angles[1] = yaw;
        angles[2] = 0;
    }

	QAngle CalcAngle1(Vector src, Vector dst)
	{
		QAngle ret;

		Vector delta = src - dst;
		double hyp = delta.Length2D(); //delta.Length
		ret.yaw = (atan(delta.y / delta.x) * 57.295779513082f);
		ret.pitch = (atan(delta.z / hyp) * 57.295779513082f);
		ret[2] = 0.00;

		if (delta.x >= 0.0)
			ret.yaw += 180.0f;

		return ret;
	}
    //--------------------------------------------------------------------------------
    static bool screen_transform(const Vector& in, Vector& out)
    {
        static auto& w2sMatrix = g_EngineClient->WorldToScreenMatrix();

        out.x = w2sMatrix.m[0][0] * in.x + w2sMatrix.m[0][1] * in.y + w2sMatrix.m[0][2] * in.z + w2sMatrix.m[0][3];
        out.y = w2sMatrix.m[1][0] * in.x + w2sMatrix.m[1][1] * in.y + w2sMatrix.m[1][2] * in.z + w2sMatrix.m[1][3];
        out.z = 0.0f;

        float w = w2sMatrix.m[3][0] * in.x + w2sMatrix.m[3][1] * in.y + w2sMatrix.m[3][2] * in.z + w2sMatrix.m[3][3];

        if(w < 0.001f) {
            out.x *= 100000;
            out.y *= 100000;
            return false;
        }

        out.x /= w;
        out.y /= w;

        return true;
    }
    //--------------------------------------------------------------------------------
    bool WorldToScreen(const Vector& in, Vector& out)
    {
        if(screen_transform(in, out)) {
            int w, h;
            g_EngineClient->GetScreenSize(w, h);

            out.x = (w / 2.0f) + (out.x * w) / 2.0f;
            out.y = (h / 2.0f) - (out.y * h) / 2.0f;

            return true;
        }
        return false;
    }
	//--------------------------------------------------------------------------------
	void inline SinCos(float radians, float *sine, float *cosine)
	{
#if defined( _X360 )
		XMScalarSinCos(sine, cosine, radians);
#elif defined( PLATFORM_WINDOWS_PC32 )
		_asm
		{
			fld DWORD PTR[radians]
			fsincos

			mov edx, DWORD PTR[cosine]
			mov eax, DWORD PTR[sine]

			fstp DWORD PTR[edx]
			fstp DWORD PTR[eax]
		}
#elif defined( PLATFORM_WINDOWS_PC64 )
		*sine = sin(radians);
		*cosine = cos(radians);
#elif defined( POSIX )
		register double __cosr, __sinr;
		__asm ("fsincos" : "=t" (__cosr), "=u" (__sinr) : "0" (radians));

		*sine = __sinr;
		*cosine = __cosr;
#endif
	}
    //--------------------------------------------------------------------------------
	void AngleMatrix(const QAngle &angles, matrix3x4_t& matrix)
	{
		float sr, sp, sy, cr, cp, cy;

#ifdef _X360
		fltx4 radians, scale, sine, cosine;
		radians = LoadUnaligned3SIMD(angles.Base());
		scale = ReplicateX4(M_PI_F / 180.f);
		radians = MulSIMD(radians, scale);
		SinCos3SIMD(sine, cosine, radians);

		sp = SubFloat(sine, 0);	sy = SubFloat(sine, 1);	sr = SubFloat(sine, 2);
		cp = SubFloat(cosine, 0);	cy = SubFloat(cosine, 1);	cr = SubFloat(cosine, 2);
#else
		SinCos(DEG2RAD(angles[0]), &sy, &cy);
		SinCos(DEG2RAD(angles[1]), &sp, &cp);
		SinCos(DEG2RAD(angles[2]), &sr, &cr);
#endif

		// matrix = (YAW * PITCH) * ROLL
		matrix[0][0] = cp*cy;
		matrix[1][0] = cp*sy;
		matrix[2][0] = -sp;

		float crcy = cr*cy;
		float crsy = cr*sy;
		float srcy = sr*cy;
		float srsy = sr*sy;
		matrix[0][1] = sp*srcy - crsy;
		matrix[1][1] = sp*srsy + crcy;
		matrix[2][1] = sr*cp;

		matrix[0][2] = (sp*crcy + srsy);
		matrix[1][2] = (sp*crsy - srcy);
		matrix[2][2] = cr*cp;

		matrix[0][3] = 0.0f;
		matrix[1][3] = 0.0f;
		matrix[2][3] = 0.0f;
	}
}


































































































