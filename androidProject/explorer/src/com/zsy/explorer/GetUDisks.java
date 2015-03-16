package com.zsy.explorer;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class GetUDisks {
	public static ArrayList<String> getUDiskDirs() {
		ArrayList<String> list = new ArrayList<String>();
		try {
			BufferedReader br = new BufferedReader(new FileReader("/proc/mounts"));
			String line = null;
			try {
				while ((line=br.readLine())!=null) {
					if (line.startsWith("/dev/block/vold/8:")) {
						System.out.println(line);
						list.add(line.split(" ")[1]);
					}
				}
			} catch (IOException e) {
				e.printStackTrace();
			}
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return list;
	}
}
